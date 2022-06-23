#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "struct.h"

/* El fet de fer servir variables 'static' fa que les
 * variables que hi ha a continuacio nomes es puguin
 * fer servir des d'aquest fitxer. Implementeu tot el
 * codi en aquest fitxer! */
static p_meta_data first_element = NULL;
static p_meta_data last_element  = NULL;

/* La seguent variable es la que permet bloquejar un 
 * codi en cas que hi hagi multiples fils. Amb aixo ens
 * assegurem que nomes un fil pugui entrar dintre d'un
 * codi que es bloqueja. Se us demana no tocar la part
 * bloquejada si voleu provar el codi amb executables
 * com el kate, kwrite, ... */
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define ALIGN8(x) (((((x)-1)>>3)<<3)+8)
#define MAGIC     0x87654321

/* Aquesta funcio permet buscar dintre de la llista un block lliure
 * que tingui la mida especificada */
p_meta_data search_available_space(size_t size_bytes) {
    p_meta_data current = first_element;

    while (current && !(current->available && current->size_bytes >= size_bytes)) 
        current = current->next;

    return current;
}

/* Aquesta funcio es la que fa la crida a sistema sbrk per demanar
 * al sistema operatiu un determinat nombre de bytes. Al principi del
 * tot s'afegiran les metadades, que l'usuari no fa servir. */
p_meta_data request_space(size_t size_bytes) 
{
    p_meta_data meta_data;

    meta_data = (void *) sbrk(0);
	
	if(size_bytes < 122880){
		size_bytes = 122880;
	}

    if (sbrk(SIZE_META_DATA + size_bytes) == (void *) -1)
        return (NULL);

    meta_data->size_bytes = size_bytes;
    meta_data->available = 0;
    meta_data->magic = MAGIC;
    meta_data->next = NULL;
    meta_data->previous = NULL;

    return meta_data;
}

/* La función setDeletedBlock es altera los punteros 
 * despues del bloque haber sido borrado. */
void setDeletedBlock (p_meta_data block) {
    block-> magic = 0;
    if (block-> previous) {
        block-> previous -> next = block -> next;
    }
    if (block-> next) {
        block -> next -> previous = block -> previous;
        if (block == first_element) {
            first_element = block -> next;
            first_element -> previous = NULL;
        }
    } else if (block->previous) last_element = block->previous;
}

/* La función insertBlock insire el nuevo bloque despues de que
 * se ha ocurrido la fusión. */
void insertBlock(p_meta_data block) {
    block-> magic = MAGIC;
    
    if (first_element == block) {
        return;
    }
    
    p_meta_data curr = first_element;
    p_meta_data prev = NULL;
    int inserted = 0;
      
    while(inserted == 0 && curr) {
    	// Recorremos la memoria
        if (block-> size_bytes > curr-> size_bytes) {
            prev = curr;
            curr = curr-> next;
        }
        // Cuando haya espacio, inserimos el bloque
        else {
            // Si hay espacio al comienzo, lo inserimos
            if (prev == NULL) {
                first_element = block;
                first_element-> previous = NULL;
                first_element-> next = curr;
                inserted = 1;
            }
            // Sino, inserimos al medio
            else {
                prev-> next = block;
                block-> previous = prev;
                block-> next = curr;
                inserted = 1;
            }
        }
    }
    // Si no hemos inserido, inserimos al final
    if (inserted == 0) {
    	if (prev == NULL) {
    	    prev-> next = block;
    	}
    	block-> previous = prev;
    	block-> next = NULL;
    	last_element = block;
    }
    fprintf(stderr, "Bloc inserit\n");
}

/* La función splitBlock divide el bloque en dos
 * si sobra un espacio mayor que 12288 bytes. */
void splitBlock(p_meta_data block, size_t size_bytes) {
	//Comprobar que sobra suficiente espacio
	if(block->size_bytes - size_bytes >= 12288){
		// Crear el nuevo bloque y tamaño
		p_meta_data ptr;
		size_t new_size = block->size_bytes - size_bytes;
		
		//Asignar los tamaños
		block->size_bytes = size_bytes;
		ptr = (p_meta_data) ((void*) block + (block->size_bytes + SIZE_META_DATA));
		
		ptr->size_bytes = new_size;
		//fprintf(stderr, "AAAAAAA\n");
		//Reordenar los bloques
		
		if(block != last_element){
			ptr->next = block->next;		
			ptr->previous = block;
			ptr->next->previous = ptr;
			block->next = ptr;
			
		}else{
			ptr->next = NULL;
			ptr->previous = block;
			block->next = ptr;
			last_element = ptr;
		}
		
		ptr->available = 1;
		ptr->magic = MAGIC;
		fprintf(stderr, "Bloc dividit en 2 trossos de %zu i de %zu\n", size_bytes, new_size);
	}	
}



/* La función fusion reune los bloques que estén libre.
 * Esta función se llama al hacermos un free(). */
void fusion(p_meta_data meta_data) {
    p_meta_data new_block = meta_data;
    p_meta_data last_fusion;
	p_meta_data p_meta_data_next;
    int fusioned = 0;

    // Si el bloque no era el ultimo de la memoria
    if (meta_data != last_element) {
        p_meta_data_next = (p_meta_data) meta_data->next;
        // Otro modo: p_meta_data next = (p_meta_data) ((void*) meta_data + (meta_data->size_bytes + SIZE_META_DATA));
	
	// Si el siguiente esta libre, fusionamos los bloques
        if (meta_data->next->available == 1) {
            meta_data->size_bytes += (meta_data->next->size_bytes + SIZE_META_DATA);
            setDeletedBlock(meta_data->next);
            fusioned = 1; 
            fprintf(stderr, "Bloc ajuntat amb el seguent\n");
	    
			// Si es el caso, actualizamos el ultimo elemento
			if (meta_data->next == last_element){
				last_element = meta_data;
			}
		}
	}
    
    // Si el bloque no era el primero de la memoria
    if (meta_data != first_element) {
        p_meta_data prev = (p_meta_data) meta_data->previous;
    	
    	// Si el anterior esta libre, fusionamos los bloques
        if (prev->available == 1) {
            prev->size_bytes += (meta_data->size_bytes + SIZE_META_DATA);
            //setDeletedBlock(prev);
            new_block = prev;
            fusioned = 1;
            fprintf(stderr, "Bloc ajuntat amb el anterior\n");
            
            // Si es el caso, actualizamos el primero
            if (prev == first_element){
				first_element = prev;
            }
        }
        
        // Si ha ocurrido una fuión, inserimos el nuevo bloque 
        if (fusioned == 1) {
            setDeletedBlock(meta_data);
            insertBlock(new_block);
        }
    }	
}


/* Allibera un bloc. L'argument es el punter del bloc a alliberar.
 * La funcio free accedeix a les metadades per indicar que el bloc
 * es lliure. */
void free(void *ptr)
{
    p_meta_data meta_data;

    if (!ptr) 
        return;
    
    // Bloquegem perque nomes hi pugui entrar un fil
    pthread_mutex_lock(&mutex);
    meta_data = (p_meta_data) (ptr - SIZE_META_DATA);

    if (meta_data->magic != MAGIC) {
      fprintf(stderr, "ERROR free: value of magic not valid\n");
      exit(1);
    } 

    meta_data->available = 1;
    fprintf(stderr, "Free at %x: %zu bytes\n", meta_data, meta_data->size_bytes);
     
    // Desbloquegem aqui perque altres fils puguin entrar
    // a la funcio
    pthread_mutex_unlock(&mutex);
    
    // Llamamos a la función fusion()
    fusion(meta_data);
}

/* La funcio malloc per demanar un determinat nombre de bytes. La
 * funcio busca primer a la seva llista si hi ha algun bloc lliure
 * de la mida demanada. Si el troba, el retorna a l'usuari. Si no
 * en troba cap de la mida demanada, fa una crida a request_space
 * la qual internament fa una crida a sistema. */
void *malloc(size_t size_bytes) 
{
  void *p, *ptr;
  p_meta_data meta_data;

  if (size_bytes <= 0) {
    return NULL;
  }

  // Reservem una mida de memoria multiple de 8. Es
  // cosa de l'electronica de l'ordinador que requereix
  // que les dades estiguin alineades amb multiples de 8.
  size_bytes = ALIGN8(size_bytes);
  fprintf(stderr, "Malloc %zu bytes\n", size_bytes);

  // Bloquegem perque nomes hi pugui entrar un fil
  pthread_mutex_lock(&mutex);

  meta_data = search_available_space(size_bytes);
  
  if (meta_data) { // free block found 
    meta_data->available = 0;
	fprintf(stderr, "Bloc de %zu bytes\n", meta_data->size_bytes);
	splitBlock(meta_data, size_bytes);
  } else {     // no free block found 
    meta_data = request_space(size_bytes);
    if (!meta_data)
		return (NULL);

	fprintf(stderr, "Bloc de %zu bytes\n", meta_data->size_bytes);
	

    if (last_element) // we add to the last element of the list
		last_element->next = meta_data;
	meta_data->previous = last_element;
	last_element = meta_data;
	

    if (first_element == NULL) // Is this the first element ?
		first_element = meta_data;
	splitBlock(meta_data, size_bytes);
  }
  
  

  p = (void *) meta_data;

  // Desbloquegem aqui perque altres fils puguin entrar
  // a la funcio
  pthread_mutex_unlock(&mutex);

  // Retornem a l'usuari l'espai que podra fer servir.
  ptr = p + SIZE_META_DATA;
  return ptr; 
}

/* La función calloc permite reservar "nelem" de memoria de
 * tamaño "elsize" y iniciarlos a 0.*/
void *calloc(size_t nelem, size_t elsize) {
    void *ptr = malloc(nelem * elsize);
    int i;
    
    for (i = 0; i < nelem; i++) {
       memset(ptr,0,elsize);
       ptr = ptr++;
    }
    
    return ptr;

}

/* La función reaalloc reajusta el tamaño de un bloque de
 * memoria a un nuevo tamaño. */
void *realloc(void* ptr, size_t size_bytes) {
    p_meta_data meta_data;
    meta_data = (p_meta_data) (ptr - SIZE_META_DATA);
    // A) Si el punter es NULL, realloc atua como malloc
    if (ptr == NULL) {
	fprintf(stderr, "Punter en NULL, fent un malloc\n");
        return malloc(size_bytes);
    }    
    // B) Si el tamaño ya era suficiente, retornamos el puntero
    else if (meta_data->size_bytes >= size_bytes && meta_data->magic == MAGIC) {
	fprintf(stderr, "Mida suficient amb el bloc que ja té reservat\n");
        return ptr;
    }
    // C) Sino, rerservamos un nuevo bloque con más espacio
    //     y copiamos el contenido al bloque actual
    else {
        void *pointer = malloc(size_bytes);
        memcpy(pointer,ptr,size_bytes);
        free(ptr);
	fprintf(stderr, "Espai insuficient per aquest punter, fent un nou malloc\n");
        return pointer;
    }
    
}

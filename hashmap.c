#include "hashmap.h"

int hash(HashMap * hm, char * key) {
	int sum = 0;
	int len = strlen(key) + 1;

	for (int i = 0; i < len; i++) {
		sum = sum + key[i];
	}

	return sum % hm->size;
}

HashMap * create_hashmap(size_t key_size) {
	assert(key_size > 0);

	HashMap * hm = (HashMap *) malloc(sizeof(HashMap));
	assert(hm);

	hm->buckets = calloc(key_size, sizeof(Bucket));
	assert(hm->buckets);

	hm->size  = key_size; 

	return hm;
}


void insert_data(HashMap * hm, char * key, void * data, ResolveCollisionCallback resolve_collision) {
	Bucket * buck = (void *) hm->buckets + (hash(hm,key)*sizeof(Bucket));
	if (buck->key != NULL && resolve_collision != NULL) {
		data = (*resolve_collision)(buck->data, data);
	}	
	buck->data = data;
	buck->key = key;
	printf("W%d\t%s\t%s\n", hash(hm,key), buck->key, (char *)buck->data);
}

void * get_data(HashMap * hm, char * key) {
	Bucket * buck = (void * ) hm->buckets + (hash(hm,key)*sizeof(Bucket));
 	printf("R%d\t%s\t%s\n", hash(hm,key), buck->key, (char *)buck->data);	
	return buck->data;
}

void iterate(HashMap * hm, void (* callback)(char * key, void * data)) {
	Bucket * buck = (void *) hm->buckets;
	for (int i = 0; i < hm->size; i++) {
		buck = (void *) hm->buckets + (i*sizeof(Bucket));
		if (buck->key != NULL) {
			(*callback)(buck->key,buck->data);
		}
	}
}

void remove_data(HashMap * hm, char * key, DestroyDataCallback destroy_data) {
	Bucket * buck = (void *) hm->buckets + (hash(hm,key)*sizeof(Bucket));
	if (buck->next != NULL) {
		;//
	}	
	if (destroy_data != NULL) {
		(*destroy_data)((void *) buck->data);
	}
	buck->data = NULL;
	buck->key = NULL;
	printf("W%d\t%s\t%s\n", hash(hm,key), buck->key, (char *)buck->data);
}

void delete_hashmap(HashMap * hm, DestroyDataCallback destroy_data) {
	if (*destroy_data != NULL){
		iterate(hm, (*destroy_data));
	}
	free(hm->buckets);
}

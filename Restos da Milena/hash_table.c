
#define SIZE_TABLE 256

typedef struct _element {
	int *value;
} element;

typedef struct _hash_table {
	element *table[SIZE_TABLE];
} hash_table;

hash_table* create_hash_table () {
	hash_table *new_ht = (hash_table*) malloc (sizeof (hash_table));
	for (int i = 0; i < SIZE_TABLE; i++) {
		new_ht->table[i] = NULL;
	}
	return new_ht;
}

void put (hash_table *ht, int key, int *value) {
	element *new_element = (element*) malloc (sizeof (element));
	new_element->value = value;
	ht->table[key] = new_element;
}

void get (hash_table *ht, int key) {
	return ht->table[key]->value;
}
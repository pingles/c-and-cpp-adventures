#include <db.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Compile: gcc src/bdb.c -o bdb -Wall -ldb */

int
store_data(DB *db, char *description, float money)
{
  int ret;
  DBT key, data;

  memset(&key, 0, sizeof(DBT));
  memset(&data, 0, sizeof(DBT));

  key.data = &money;
  key.size = sizeof(float);

  data.data = description;
  data.size = strlen(description) + 1;

  ret = db->put(db, NULL, &key, &data, DB_NOOVERWRITE);
  if (ret == DB_KEYEXIST) {
    db->err(db, ret, "Put failed, key %f already exists", money);
  }

  return 0;
}

int
read_data(DB *db, float *key, char *description)
{
  int ret;
  DBT k, d;

  memset(&k, 0, sizeof(DBT));
  memset(&d, 0, sizeof(DBT));

  k.data = key;
  k.size = sizeof(float);
  
  d.data = description;
  d.ulen = 200;
  d.flags = DB_DBT_USERMEM;

  ret = db->get(db, NULL, &k, &d, 0);
  if (ret != 0) {
    db->err(db, ret, "Get failed.", NULL);
  }

  return ret;
}

int
main(int argc, char *argv[])
{
  if (argc < 1) {
    printf("Specify DB name\n");
    return -1;
  }

  
  DB *dbp;
  int ret;

  ret = db_create(&dbp, NULL, 0);

  if (ret !=0) {
    printf("Error creating DB handle\n");
  }

  int file_mode = 0;

  printf("Opening DB: %s\n", argv[1]);

  ret = dbp->open(dbp, NULL, argv[1], NULL, DB_BTREE, DB_CREATE, file_mode);
  if (ret != 0) {
    printf("Error opening DB\n");
  }

  float result_key = 122.56;
  char result_desc[200];

  ret = read_data(dbp, &result_key, result_desc);
  if (ret == 0) {
    printf("--------- reading ---------------\n");
    printf("Key : %f\n", result_key);
    printf("Data: %s\n", result_desc);
    printf("--------- reading ---------------\n");
  }

  store_data(dbp, "Grocery Bill", 122.56);

  ret = dbp->close(dbp, 0);
  if (ret != 0) {
    printf("Error closing DB\n");
  }

  return EXIT_SUCCESS;
}

#include <db.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Compile: gcc src/bdb.c -o bdb -Wall -ldb */

int
store_data(DB *db, char* description, float money)
{
  int ret;
  DBT key, data;

  memset(&key, 0, sizeof(DBT));
  memset(&data, 0, sizeof(DBT));

  key.data = &money;
  key.size = sizeof(float);

  data.data = &description;
  data.size = strlen(description) + 1;

  ret = db->put(db, NULL, &key, &data, DB_NOOVERWRITE);
  if (ret == DB_KEYEXIST) {
    db->err(db, ret, "Put failed, key %f already exists", money);
  }

  return 0;
}

int
main(void)
{
  DB *dbp;
  int ret;

  ret = db_create(&dbp, NULL, 0);

  if (ret !=0) {
    printf("Error creating DB handle\n");
  }

  int file_mode = 0;

  ret = dbp->open(dbp, NULL, "./my_db.db", NULL, DB_BTREE, DB_CREATE, file_mode);
  if (ret != 0) {
    printf("Error opening DB\n");
  }

  float money = 122.56;
  char *description = "Grocery Bill";

  store_data(dbp, description, money);

  ret = dbp->close(dbp, 0);
  if (ret != 0) {
    printf("Error closing DB\n");
  }

  return EXIT_SUCCESS;
}

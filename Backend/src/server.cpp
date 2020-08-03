/**
 * test.c
 * Small Hello World! example
 * to compile with gcc, run the following command
 * gcc -o test test.c -lulfius
 */
#ifdef __cplusplus
extern "C"{
#endif
#include <ulfius.h>
#include <yder.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#define PORT 1717
#define PREFIX "/api"
#define FILE_PREFIX "/upload"
#ifdef __cplusplus
}
#endif

/**
 * Definition of callbacks 
 */
int callback_hello_world (const struct _u_request * request, struct _u_response * response, void * user_data);
int callback_upload_file (const struct _u_request * request, struct _u_response * response, void * originalPath);
int file_upload_callback (const struct _u_request * request, 
                          const char * key, 
                          const char * filename, 
                          const char * content_type, 
                          const char * transfer_encoding, 
                          const char * data, 
                          uint64_t off, 
                          size_t size, 
                          void * cls);

/**
 * main function
 */
int main(void) {

  //initializes instance of the aerver
  struct _u_instance instance;

  // Initialize instance with the port number
  if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
    fprintf(stderr, "Error ulfius_init_instance, abort\n");
    return(1);
  }

  // Max post param size is 16 Kb, which means an uploaded file is no more than 16 Kb
  instance.max_post_param_size = 10000*1024;


  // Endpoint list declaration
  ulfius_add_endpoint_by_val(&instance, "GET", PREFIX, "/hello", 0, &callback_hello_world, NULL);
  ulfius_add_endpoint_by_val(&instance, "*", FILE_PREFIX, NULL, 0, &callback_upload_file, NULL);

  if (ulfius_set_upload_file_callback_function(&instance, &file_upload_callback, NULL) != U_OK) {
    printf("Error ulfius_set_upload_file_callback_function\n");
  }

  // Start the framework
  if (ulfius_start_framework(&instance) == U_OK) {
    printf("Start framework on port %d\n", instance.port); 

    while(1){
      sleep(100);
    }
  } else {
    fprintf(stderr, "Error starting framework\n");
  }

  printf("End framework\n");


  ulfius_stop_framework(&instance);
  ulfius_clean_instance(&instance);

  return 0;
}

/**
 * Callback function for the web application on /helloworld url call
 */

int callback_hello_world (const struct _u_request * request, struct _u_response * response, void * user_data) {
  ulfius_set_string_body_response(response, 200, "Hello World!");
  return U_CALLBACK_CONTINUE;
}

/**
 * upload a file
 */
int callback_upload_file(const struct _u_request * request, struct _u_response * response, void * user_data) {
   //Paths
  const char **keys,*fileName, *fileContent, *filepath;

  filepath = "../assets/images/image.png";

  printf("Hola de nuevo\n");
  // system((const char *)"rm ../assets/images/image.png");

  return U_OK;
}


int file_upload_callback (const struct _u_request * request, 
                          const char * key, 
                          const char * filename, 
                          const char * content_type, 
                          const char * transfer_encoding, 
                          const char * data, 
                          uint64_t off, 
                          size_t size, 
                          void * cls) {
  const char * filepath = "../assets/images/image.png";

  FILE * file;

  printf("Escribiendo %d bytes\n",size);
  file=fopen(filepath,"ab");
  fwrite(data, size, 1, file);
  fclose(file);
  return U_OK;
}

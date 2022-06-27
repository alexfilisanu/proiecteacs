#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *query_params,
    char **cookies, int cookies_count, char *jwt)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
	if (query_params != NULL) {
		sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
	} else {
		sprintf(line, "GET %s HTTP/1.1", url);
	}

	compute_message(message, line);

    // Step 2: add the host
    memset(line, 0, strlen(line));
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (cookies != NULL) {
		memset(line, 0, LINELEN);
        strcat(line, "Cookie: ");

        for (int i = 0; i < cookies_count - 1; i++) {
            strcat(line, cookies[i]);
            strcat(line, ";");
        }

        strcat(line, cookies[cookies_count - 1]);
        compute_message(message, line);
    }

	if(jwt != NULL) {
        memset(line, 0, strlen(line));
		sprintf(line, "Authorization: Bearer %s", jwt);
		compute_message(message, line);
	}

    // Step 4: add final new line
    compute_message(message, "\n");
    return message;
}

char *compute_delete_request(char *host, char *url, char *query_params,
    char **cookies, int cookies_count, char *jwt)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
	if (query_params != NULL) {
		sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
	} else {
		sprintf(line, "DELETE %s HTTP/1.1", url);
	}
    compute_message(message, line);

    // Step 2: add the host
    memset(line, 0, strlen(line));
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (cookies != NULL) {
		memset(line, 0, LINELEN);
        strcat(line, "Cookie: ");

        for (int i = 0; i < cookies_count - 1; i++) {
            strcat(line, cookies[i]);
            strcat(line, ";");
        }

        strcat(line, cookies[cookies_count - 1]);
        compute_message(message, line);
    }

	if(jwt != NULL) {
        memset(line, 0, strlen(line));
		sprintf(line, "Authorization: Bearer %s", jwt);
		compute_message(message, line);
	}

    // Step 4: add final new line
    compute_message(message, "");
    return message;
}

char *compute_post_request(char *host, char *url, char *content_type, char **body_data,
    int body_data_fields_count, char **cookies, int cookies_count, char *jwt)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    char *body_data_buffer = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL and protocol type
	sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);

	if(jwt != NULL) {
        memset(line, 0, strlen(line));
		sprintf(line, "Authorization: Bearer %s", jwt);
		compute_message(message, line);
	}
    // Step 2: add the host
    memset(line, 0, strlen(line));
    sprintf(line, "Host: %s", host);
    compute_message(message, line);


    // Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    if (body_data != NULL) {
		memset(body_data_buffer, 0, LINELEN);
    	strcat(body_data_buffer, "");
    	strcat(body_data_buffer, body_data[0]);
	}

    memset(line, 0, LINELEN);
    sprintf(line, "Content-Length: %ld", strlen(body_data_buffer));
    compute_message(message, line);

    // Step 4 (optional): add cookies
    if (cookies != NULL) {
        compute_message(message, line);
    }

    // Step 5: add new line at end of header
    strcat(message, "\n");

    // Step 6: add the actual payload data
	if (body_data != NULL) {
	    compute_message(message, body_data_buffer);
	}

    free(line);
    return message;
}
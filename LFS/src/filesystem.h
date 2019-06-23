/*
 * filesystem.h
 *
 *  Created on: 15 jun. 2019
 *      Author: utnso
 */

#ifndef FILESYSTEM_H_
	#define FILESYSTEM_H_

	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <dirent.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <commons/string.h>
	#include <errno.h>
	#include <time.h>
	#include <inttypes.h>

	#include <shared/utils.h>
	#include "shared/protocolo.h"
	#include "config/config_LFS.h"
	#include "filesystem.h"
	#include "memtable.h"

	#define TABLES_FOLDER "tables/"

	void crearParticiones(char* table_name, int partitions, int* ok);
	void crearTemporal(char* table_name,char* temporal, int* ok);
	void obtenerDatos(char* pathParticion, char** ret_buffer, int* ret_buffer_size);
	void guardarDatos(char* pathParticion, int bytes, void* buffer, int* ok);
	char* get_partition_for_key(char* table_name, char* key);
	void borrar_archivo(char* path, int* ok);
	char* get_last_value(t_list* registros);
	t_list* filter_registro_list_by_key(t_list* list, char* key);
	t_list* buffer_to_list_registros(char* buffer);

#endif /* FILESYSTEM_H_ */

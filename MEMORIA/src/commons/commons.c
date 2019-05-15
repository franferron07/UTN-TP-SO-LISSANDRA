/*
 * config.c
 *
 *  Created on: 11 abr. 2019
 *      Author: utnso
 */

#include "commons.h"

int mem_initialize() {

	config = config_create(pathCFG);
	crear_log();

	if (config == NULL) {
		log_error(mem_log, "Error al leer ruta del archivo de configuracion");
		return -1;
	}
	log_info(mem_log, ".:: LISSANDRA-MEMORIA ::.");
	log_info(mem_log, ".:: Cargando configuracion ::.");

	mem_config.puerto_mem = strdup(config_get_string_value(config, "PUERTO"));
	mem_config.ip_LFS = strdup(config_get_string_value(config, "IP_FS"));
	mem_config.puerto_LFS = strdup(config_get_string_value(config, "PUERTO_FS"));
	mem_config.ip_SEEDS = config_get_array_value(config, "IP_SEEDS");
	mem_config.puerto_SEEDS = config_get_array_value(config, "PUERTO_SEEDS");
	mem_config.retardo_mem = config_get_int_value(config, "RETARDO_MEM");
	mem_config.retardo_fs = config_get_int_value(config, "RETARDO_FS");
	mem_config.tam_mem = config_get_int_value(config, "TAM_MEM");
	mem_config.retardo_journal = config_get_int_value(config,"RETARDO_JOURNAL");
	mem_config.retardo_gossiping = config_get_int_value(config,"RETARDO_GOSSIPING");
	mem_config.memory_number = config_get_int_value(config, "MEMORY_NUMBER");

	config_destroy(config);

	return 1;

}

void imprimir_config() {
	log_info(mem_log, ".:: Imprimiendo configuracion ::.");
	log_info(mem_log, "PUERTO MEMORIA: %s", mem_config.puerto_mem);
	log_info(mem_log, "IP FLS: %s", mem_config.ip_LFS);
	log_info(mem_log, "PUERTO FLS: %s", mem_config.puerto_LFS);

	imprimir_arrays(mem_config.ip_SEEDS,"ARRAY DE IP SEEDS");
	imprimir_arrays(mem_config.puerto_SEEDS,"ARRAY DE PUERTO SEEDS");

	log_info(mem_log, "RETARDO_MEM: %d", mem_config.retardo_mem);
	log_info(mem_log, "RETARDO_FS: %d", mem_config.retardo_fs);
	log_info(mem_log, "TAM_MEM: %d", mem_config.tam_mem);
	log_info(mem_log, "RETARDO_JOURNAL: %d", mem_config.retardo_journal);
	log_info(mem_log, "RETARDO_GOSSIPING: %d", mem_config.retardo_gossiping);
	log_info(mem_log, "MEMORY_NUMBER: %d", mem_config.memory_number);
}

void crear_log() {
	mem_log = log_create(pathLOG, "LISSANDRA-MEMORIA", false, LOG_LEVEL_TRACE);
	if (mem_log == NULL) {
		printf("No se pudo crear el log. Abortando ejecución\n");
		exit(EXIT_FAILURE);
	}
}

void imprimir_arrays(char** split,char* nombre)
{
	unsigned int i = 0;
	log_info(mem_log, "%s", nombre);
	for(; split[i] != NULL;i++){
		log_info(mem_log, "%d: %s",i,split[i]);
	}
}

void liberar_tablas() {
	liberar_tabla_segmentos(tabla_segmentos);
}

void liberar_mem_config(mem_cfg mem_config)
{
	free(mem_config.puerto_mem);
	free(mem_config.ip_LFS);
	free(mem_config.puerto_LFS);
	split_liberar(mem_config.ip_SEEDS);
	split_liberar(mem_config.puerto_SEEDS);
}

void liberar_memoria_contigua(){
	free(memoria);
	log_info(mem_log, "LIBERADO MEMORIA CONTIGUA");

}

void liberar_fila_paginas(fila_TPaginas* fila_pagina)
{
	free(fila_pagina);
}

void liberar_tabla_paginas(fila_TSegmentos segmento){
	list_iterate(segmento.paginas,(void*)liberar_fila_paginas);
	list_destroy(segmento.paginas);
	log_info(mem_log, "LIBERADO TABLA DE PAGINAS");

}

void liberar_tabla_segmentos(t_list* tabla_segmentos) {
	list_iterate(tabla_segmentos,(void*)liberar_tabla_paginas);
	list_destroy(tabla_segmentos);
	log_info(mem_log, "LIBERADO TABLA DE SEGMENTOS");
}

void mem_exit_global() {
	liberar_tablas();
	liberar_memoria_contigua();
	mem_exit_simple();
}

void mem_exit_simple() {
	liberar_mem_config(mem_config);
	log_info(mem_log, "[MEMORIA] LIBERO MEMORIA CONFIG");
	log_destroy(mem_log);
}



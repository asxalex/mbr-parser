/*
 * parted.c
 * Copyright (C) 2016 alex <alex@alex>
 *
 * Distributed under terms of the MIT license.
 */

#include <stdio.h>
#include <stdlib.h>
#ifdef MODIFY
#include <string.h>
#endif

#pragma pack(1)

#define SECTOR_SIZE 512
#define PROG_LENGTH 446
#define MEGA_BYTES (1024*1024)

typedef struct _chs_s chs_t;
typedef struct _parted_s partition_t;
typedef struct _mbr_s mbr_t;

struct _chs_s {
    unsigned char head;
    unsigned short sector:6;
    unsigned short cylinder:10;
};

struct _parted_s {
    unsigned char active;
    chs_t start;
    unsigned char type;
    chs_t end;
    unsigned int start_sector;
    unsigned int sector_num;
};

struct _mbr_s {
    unsigned char program[PROG_LENGTH];
    partition_t partition[4];
    unsigned char end_flag[2];
};

void print_chs(chs_t chs) {
    printf("\thead=%d\n", chs.head);
    printf("\tsector=%d\n", chs.sector);
    printf("\tcylinder=%d\n", chs.cylinder);
}

void print_partition(partition_t *part) {
    printf("active: %x\n", part->active);
    printf("type: %x\n", part->type);
    printf("-> start CHS info:\n");
    print_chs(part->start);
    printf("start_sector: %d\n", part->start_sector);
    printf("-> end CHS info:\n");
    print_chs(part->end);
    printf("sector_num: %d\n", part->sector_num);
    printf("partition size = %ld (MB)\n", (unsigned long)part->sector_num * SECTOR_SIZE / MEGA_BYTES);
    printf("\n");
}


int main(int argc, char *argv[]) {
    
    /* open the filename */
    char *filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        filename = "/dev/sda";
    }
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "failed to open file [%s]\n", filename);
        exit(-1);
    }

    /* read the file content to the structure */
    mbr_t mbr;
    fread(&mbr, sizeof(mbr_t), 1, fp);
    fclose(fp);
    for(int i = 0; i < 4; i++) {
        // print partition information
        print_partition(&(mbr.partition[i]));
    }
    // 0x55aa
    printf("%x%x\n", mbr.end_flag[0], mbr.end_flag[1]);

#ifdef MODIFY
    /* try to change the mbr's partition table
     * all the partitions are primary partition */
    FILE *target = fopen("result", "w");
    if (!target) {
        fprintf(stderr, "failed to open the file result\n");
    }  else {
        // write the bootloader
        fwrite(mbr.program, PROG_LENGTH, 1, target);

        // calculate the partition 3
        mbr.partition[2].end = mbr.partition[3].end;
        mbr.partition[2].sector_num += mbr.partition[3].sector_num;

        // zeor-rize the partition 4
        memset(&(mbr.partition[3]), '\0', sizeof(partition_t));

        // keep the first two partition unchanged.
        // while add partition 4 to partition 3,
        // and change the partition 4 to all-zero
        fwrite(mbr.partition, sizeof(partition_t), 4, target);
        
        // write the 0x55aa
        fwrite(mbr.end_flag, sizeof(char), 2, target);

        fclose(target);
    }
#endif

    // dump the bootloader, for future use
    fp = fopen("bootloader.obj", "w");
    if (!fp) {
        fprintf(stderr, "failed to open bootloader.obj\n");
        exit(-1);
    }
    fwrite(mbr.program, PROG_LENGTH, 1, fp);
    fclose(fp);

    return 0;
}

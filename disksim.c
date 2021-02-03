#include <stdlib.h>
#include <memory.h>
#include "ext2.h"
#include "disk.h"
#include "disksim.h"

typedef struct
{
	char*	address;
} DISK_MEMORY;

int disksim_read(DISK_OPERATIONS* this, SECTOR sector, void* data);
int disksim_write(DISK_OPERATIONS* this, SECTOR sector, const void* data);

int disksim_init( SECTOR numberOfSectors, unsigned int bytesPerSector, DISK_OPERATIONS* disk ) 
{	// pdata�� main���� ��û�� disk ũ�� ��ŭ �Ҵ��ؼ� ����

	if( disk == NULL ) //����Ű�� DISK_OPERATIONS ����ü�� ���ٸ� ���� ó��
		return -1;

	disk->pdata = malloc( sizeof( DISK_MEMORY ) ); //disk.pdata�� �޸� �Ҵ�
	if( disk->pdata == NULL )// �Ҵ� ���н�
	{
		disksim_uninit( disk );		// �ش� DISK_OPERATIONS�� pdata ���� �޸� �Ҵ� ����(54 ����)
		return -1;
	}

	( ( DISK_MEMORY* )disk->pdata )->address = ( char* )malloc( bytesPerSector * numberOfSectors ); // �ش� �ּҿ� (���ͼ�*���ʹ� ����Ʈ)��ŭ �޸� �Ҵ�
	if( disk->pdata == NULL )	// �Ҵ� ���н�
	{
		disksim_uninit( disk );	// // �ش� DISK_OPERATIONS�� pdata ���� �޸� �Ҵ� ����(54 ����)
		return -1;
	}

	disk->read_sector	= disksim_read;
	disk->write_sector	= disksim_write;
	disk->numberOfSectors	= numberOfSectors;
	disk->bytesPerSector	= bytesPerSector;
	// ������ g_disk, DISK_OPERATIONS ����ü�� �Լ�, ���ͼ�, ���� �� ����Ʈ ����
	return 0;
}

void disksim_uninit( DISK_OPERATIONS* this ) // pdata�޸� �Ҵ� ����
{
	if( this )
	{
		if( this->pdata )
			free( this->pdata ); // ���ڷ� ���� this�� pdata�� ������ ��� �Ҵ� ����
	}
}

int disksim_read( DISK_OPERATIONS* this, SECTOR sector, void* data )	
{
	char* disk = ( ( DISK_MEMORY* )this->pdata )->address;  // ������ ���� disk�� DISK_OPERATIONS ����ü�� pdata �ּ� ����

	if( sector < 0 || sector >= this->numberOfSectors )		// ���ڷ� ���� sector�� 0���� �۰ų� ��ũ ����ü�� ���� ������ ������ ����ó��
		return -1;

	memcpy( data, &disk[sector * this->bytesPerSector], this->bytesPerSector ); 
	// (�Է¹��� ���͹�ȣ*���ʹ� ����Ʈ ��)�� �ּҷκ��� �� ���͸�ŭ�� ���� data�� ����
	return 0;
}

int disksim_write( DISK_OPERATIONS* this, SECTOR sector, const void* data )
{
	char* disk = ( ( DISK_MEMORY* )this->pdata )->address;	// ������ ���� disk�� DISK_OPERATIONS ����ü�� pdata �ּ� ����

	if( sector < 0 || sector >= this->numberOfSectors )	// ���ڷ� ���� sector�� 0���� �۰ų� ��ũ ����ü�� ���� ������ ������ ����ó��
		return -1;

	memcpy( &disk[sector * this->bytesPerSector], data, this->bytesPerSector ); 
	// (�Է¹��� ���͹�ȣ*���ʹ� ����Ʈ ��)�� �ּҷκ��� �� ���͸�ŭ�� data ����
	return 0;
}


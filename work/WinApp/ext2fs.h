#ifndef _EXT2FS
#define _EXT2FS

#include "windows.h"
#include "W32Lib.h"

//
// The second extended file system magic number
//
#define EXT2_SUPER_MAGIC			(WORD)0xEF53

//
// Ext2 directory file types.  Only the low 3 bits are used.  The
// other bits are reserved for now.
//
#define EXT2_FT_UNKNOWN				0
#define EXT2_FT_REG_FILE			1
#define EXT2_FT_DIR					2
#define EXT2_FT_CHRDEV				3
#define EXT2_FT_BLKDEV				4
#define EXT2_FT_FIFO				5
#define EXT2_FT_SOCK				6
#define EXT2_FT_SYMLINK				7
#define EXT2_FT_MAX					8

//
// Special inode numbers
//
#define EXT2_BAD_INO				1		// Bad blocks inode
#define EXT2_ROOT_INO				2		// Root inode
#define EXT2_ACL_IDX_INO			3		// ACL inode
#define EXT2_ACL_DATA_INO			4		// ACL inode
#define EXT2_BOOT_LOADER_INO		5		// Boot loader inode
#define EXT2_UNDEL_DIR_INO			6		// Undelete directory inode
#define EXT2_RESIZE_INO				7		// Reserved group descriptors inode
#define EXT2_JOURNAL_INO			8		// Journal inode

//
// Constants relative to the data blocks
//
#define EXT2_NDIR_BLOCKS			12
#define EXT2_IND_BLOCK				EXT2_NDIR_BLOCKS
#define EXT2_DIND_BLOCK				(EXT2_IND_BLOCK + 1)
#define EXT2_TIND_BLOCK				(EXT2_DIND_BLOCK + 1)
#define EXT2_N_BLOCKS				(EXT2_TIND_BLOCK + 1)



#pragma pack(1)

//
// Structure of a blocks group descriptor for ext2
//
struct ext2_group_desc
{
	DWORD 	bg_block_bitmap;				// Blocks bitmap block
	DWORD	bg_inode_bitmap;				// Inodes bitmap block
	DWORD	bg_inode_table;					// Inodes table block
	WORD	bg_free_blocks_count;			// Free blocks count
	WORD	bg_free_inodes_count;			// Free inodes count
	WORD	bg_used_dirs_count;				// Directories count
	WORD	bg_flags;
	DWORD	bg_reserved[2];
	WORD	bg_itable_unused;				// Unused inodes count
	WORD	bg_checksum;					// crc16(s_uuid+grouo_num+group_desc)
};

//
// Structure of a blocks group descriptor for ext4
//
struct ext4_group_desc
{
	DWORD	bg_block_bitmap;				// Blocks bitmap block
	DWORD	bg_inode_bitmap;				// Inodes bitmap block
	DWORD	bg_inode_table;					// Inodes table block
	WORD	bg_free_blocks_count;			// Free blocks count
	WORD	bg_free_inodes_count;			// Free inodes count
	WORD	bg_used_dirs_count;				// Directories count
	WORD	bg_flags;
	DWORD	bg_reserved[2];
	WORD	bg_itable_unused;				// Unused inodes count
	WORD	bg_checksum;					// crc16(s_uuid+grouo_num+group_desc)
	DWORD	bg_block_bitmap_hi;				// Blocks bitmap block MSB
	DWORD	bg_inode_bitmap_hi;				// Inodes bitmap block MSB
	DWORD	bg_inode_table_hi;				// Inodes table block MSB
	DWORD	bg_free_blocks_count_hi;		// Free blocks count MSB
	WORD	bg_free_inodes_count_hi;		// Free inodes count MSB
	WORD	bg_used_dirs_count_hi;			// Directories count MSB
	WORD	bg_pad;
	DWORD	bg_reserved2[3];
};

//
// Structure of an inode on the disk
//
struct ext2_inode
{
	WORD	i_mode;							// File mode
	WORD	i_uid;							// Low 16 bits of Owner Uid
	DWORD	i_size;							// Size in bytes
	DWORD	i_atime;						// Access time
	DWORD	i_ctime;						// Inode change time
	DWORD	i_mtime;						// Modification time
	DWORD	i_dtime;						// Deletion Time
	WORD	i_gid;							// Low 16 bits of Group Id
	WORD	i_links_count;					// Links count
	DWORD	i_blocks;						// Blocks count
	DWORD	i_flags;						// File flags
	union
	{
		struct
		{
			DWORD	l_i_version;			// was l_i_reserved1
		} linux1;
		struct
		{
			DWORD  h_i_translator;
		} hurd1;
	} osd1;									// OS dependent 1
	DWORD	i_block[EXT2_N_BLOCKS];			// Pointers to blocks
	DWORD	i_generation;					// File version (for NFS)
	DWORD	i_file_acl;						// File ACL
	DWORD	i_dir_acl;						// Directory ACL
	DWORD	i_faddr;						// Fragment address
	union
	{
		struct
		{
			WORD	l_i_blocks_hi;
			WORD	l_i_file_acl_high;
			WORD	l_i_uid_high;			// these 2 fields
			WORD	l_i_gid_high;			// were reserved2[0]
			DWORD	l_i_reserved2;
		} linux2;
		struct
		{
			BYTE	h_i_frag;				// Fragment number
			BYTE	h_i_fsize;				// Fragment size
			WORD	h_i_mode_high;
			WORD	h_i_uid_high;
			WORD	h_i_gid_high;
			DWORD	h_i_author;
		} hurd2;
	} osd2;									// OS dependent 2
};

//
// Permanent part of an large inode on the disk
//
struct ext2_inode_large {
	WORD	i_mode;							// File mode
	WORD	i_uid;							// Low 16 bits of Owner Uid
	DWORD	i_size;							// Size in bytes
	DWORD	i_atime;						// Access time
	DWORD	i_ctime;						// Inode Change time
	DWORD	i_mtime;						// Modification time
	DWORD	i_dtime;						// Deletion Time
	WORD	i_gid;							// Low 16 bits of Group Id
	WORD	i_links_count;					// Links count
	DWORD	i_blocks;						// Blocks count
	DWORD	i_flags;						// File flags
	union
	{
		struct
		{
			DWORD	l_i_version;			// was l_i_reserved1
		} linux1;
		struct
		{
			DWORD  h_i_translator;
		} hurd1;
	} osd1;									// OS dependent 1
	DWORD	i_block[EXT2_N_BLOCKS];			// Pointers to blocks
	DWORD	i_generation;					// File version (for NFS)
	DWORD	i_file_acl;						// File ACL
	DWORD	i_dir_acl;						// Directory ACL
	DWORD	i_faddr;						//  Fragment address
	union
	{
		struct
		{
			WORD	l_i_blocks_hi;
			WORD	l_i_file_acl_high;
			WORD	l_i_uid_high;			// these 2 fields
			WORD	l_i_gid_high;			// were reserved2[0]
			DWORD	l_i_reserved2;
		} linux2;
		struct
		{
			BYTE	h_i_frag;				// Fragment number
			BYTE	h_i_fsize;				// Fragment size
			WORD	h_i_mode_high;
			WORD	h_i_uid_high;
			WORD	h_i_gid_high;
			DWORD	h_i_author;
		} hurd2;
	} osd2;									// OS dependent 2
	WORD	i_extra_isize;
	WORD	i_pad1;
	DWORD	i_ctime_extra;					// extra Change time (nsec << 2 | epoch)
	DWORD	i_mtime_extra;					// extra Modification time (nsec << 2 | epoch)
	DWORD	i_atime_extra;					// extra Access time (nsec << 2 | epoch)
	DWORD	i_crtime;						// File creation time
	DWORD	i_crtime_extra;					// extra File creation time (nsec << 2 | epoch
	DWORD	i_version_hi;					// high 32 bits for 64-bit version
};

//
// Structure of the super block
//
struct ext2_super_block
{
	DWORD	s_inodes_count;					// Inodes count
	DWORD	s_blocks_count;					// Blocks count
	DWORD	s_r_blocks_count;				// Reserved blocks count
	DWORD	s_free_blocks_count;			// Free blocks count
	DWORD	s_free_inodes_count;			// Free inodes count
	DWORD	s_first_data_block;				// First Data Block
	DWORD	s_log_block_size;				// Block size
	LONG	s_log_frag_size;				// Fragment size 
	DWORD	s_blocks_per_group;				// # Blocks per group
	DWORD	s_frags_per_group;				// # Fragments per group
	DWORD	s_inodes_per_group;				// # Inodes per group
	DWORD	s_mtime;						// Mount time
	DWORD	s_wtime;						// Write time
	WORD	s_mnt_count;					// Mount count
	SHORT	s_max_mnt_count;				// Maximal mount count
	WORD	s_magic;						// Magic signature EXT2_SUPER_MAGIC
	WORD	s_state;						// File system state
	WORD	s_errors;						// Behaviour when detecting errors
	WORD	s_minor_rev_level;				// minor revision level
	DWORD	s_lastcheck;					// time of last check
	DWORD	s_checkinterval;				// max. time between checks
	DWORD	s_creator_os;					// OS
	DWORD	s_rev_level;					// Revision level
	WORD	s_def_resuid;					// Default uid for reserved blocks
	WORD	s_def_resgid;					// Default gid for reserved blocks
	DWORD	s_first_ino;					// First non-reserved inode 
	WORD	s_inode_size;					// size of inode structure
	WORD	s_block_group_nr;				// block group # of this superblock 
	DWORD	s_feature_compat;				// compatible feature set
	DWORD	s_feature_incompat;				// incompatible feature set
	DWORD	s_feature_ro_compat;			// readonly-compatible feature set
	BYTE	s_uuid[16];						// 128-bit uuid for volume
	char	s_volume_name[16];				// volume name
	char	s_last_mounted[64];				// directory where last mounted
	DWORD	s_algorithm_usage_bitmap;		// For compression

	//
	// Performance hints.  Directory preallocation should only
	// happen if the EXT2_FEATURE_COMPAT_DIR_PREALLOC flag is on.
	//
	BYTE	s_prealloc_blocks;				// Nr of blocks to try to preallocate
	BYTE	s_prealloc_dir_blocks;			// Nr to preallocate for dirs
	WORD	s_reserved_gdt_blocks;			// Per group table for online growth

	//
	// Journaling support valid if EXT2_FEATURE_COMPAT_HAS_JOURNAL set.
	//
	BYTE	s_journal_uuid[16];				// uuid of journal superblock
	DWORD	s_journal_inum;					// inode number of journal file
	DWORD	s_journal_dev;					// device number of journal file
	DWORD	s_last_orphan;					// start of list of inodes to delete
	DWORD	s_hash_seed[4];					// HTREE hash seed
	BYTE	s_def_hash_version;				// Default hash version to use
	BYTE	s_jnl_backup_type;				// Default type of journal backup
	WORD	s_desc_size;					// Group desc. size: INCOMPAT_64BIT
	DWORD	s_default_mount_opts;
	DWORD	s_first_meta_bg;				// First metablock group
	DWORD	s_mkfs_time;					// When the filesystem was created
	DWORD	s_jnl_blocks[17];				// Backup of the journal inode
	DWORD	s_blocks_count_hi;				// Blocks count high 32bits
	DWORD	s_r_blocks_count_hi;			// Reserved blocks count high 32 bits
	DWORD	s_free_blocks_hi;				// Free blocks count
	WORD	s_min_extra_isize;				// All inodes have at least # bytes
	WORD	s_want_extra_isize;				// New inodes should reserve # bytes
	DWORD	s_flags;						// Miscellaneous flags
	WORD	s_raid_stride;					// RAID stride
	WORD	s_mmp_interval;					// # seconds to wait in MMP checking
	ULONGLONG s_mmp_block;					// Block for multi-mount protection
	DWORD   s_raid_stripe_width;			// blocks on all data disks (N*stride)
	BYTE	s_log_groups_per_flex;			// FLEX_BG group size
	BYTE	s_reserved_char_pad;
	WORD	s_reserved_pad;					// Padding to next 32bits
	ULONGLONG s_kbytes_written;				// nr of lifetime kilobytes written
	DWORD   s_reserved[160];				// Padding to the end of the block
};
#pragma pack()


class Ext2Volume 
{
private:

public:
	Ext2Volume(TCHAR *file, LONGLONG offset);
	~Ext2Volume();
};



#endif // _EXT2FS
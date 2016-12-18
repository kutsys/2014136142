#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <elf.h>

#define OPT_ELF_HEADER 		0x1
#define OPT_SEC_HEADER 		0x2
#define OPT_PRO_HEADER 		0x4

#define GNU_EH_FRAME 0x6474e550
#define GNU_STACK 0x6474e551
#define GNU_RELRO 0x6474e552

#define MAX_PATH 256
#define TRUE 1
#define FALSE 0

void print_ELF32_Header(Elf32_Ehdr ehdr);
void print_ELF64_Header(Elf64_Ehdr ehdr);
void print_Program32_Header(FILE *fp, Elf32_Ehdr ehdr);
void print_Program64_Header(FILE *fp, Elf64_Ehdr ehdr);
void print_Section32_Header(FILE *fp, Elf32_Ehdr ehdr);
void print_Section64_Header(FILE *fp, Elf64_Ehdr ehdr);

int main(int argc, char **argv){
	int c, opt = 0;
	char* exec = NULL;
	while((c = getopt(argc, argv, "eps")) != -1)
		switch(c){
			case 'e':
				opt |= OPT_ELF_HEADER; break;
			case 'p':
				opt |= OPT_PRO_HEADER; break;
			case 's':
				opt |= OPT_SEC_HEADER; break;
			default:
				if (isprint(optopt))
					printf("Unknown option '-%c'.\n", optopt);
				else
					printf("Unknown option character '\\x%x'.\n", optopt);
				exit(EXIT_FAILURE);
		}
	if(opt==0) opt = OPT_ELF_HEADER;
	exec = argv[optind];

	if(exec == NULL){
		printf("Error! input file not exist\n");
		exit(EXIT_FAILURE);
	}

	FILE* fp = fopen(exec, "r");
	if(fp == NULL){
		printf("Error! file(%s) is not opened\n", exec);
		exit(EXIT_FAILURE);
	}
	
	char eident[EI_NIDENT];
	fread(eident, sizeof(char), sizeof(eident), fp);
	
	if(eident[EI_MAG0]!=0x7F || strncmp(eident+EI_MAG0+1, "ELF", EI_MAG3-EI_MAG0)) {
		printf("Error! file(%s) is not elf file\n", exec);
		exit(EXIT_FAILURE);
	}
	
	fseek(fp, 0, SEEK_SET);
	if(eident[EI_CLASS] == ELFCLASS32) {
		Elf32_Ehdr elfEhdr;
		fread(&elfEhdr, 1, sizeof(elfEhdr), fp);
		if(opt & OPT_ELF_HEADER)
			print_ELF32_Header(elfEhdr);
		if(opt & OPT_SEC_HEADER)
			print_Section32_Header(fp, elfEhdr);
		if(opt & OPT_PRO_HEADER)
			print_Program32_Header(fp, elfEhdr);
	}
	else if(eident[EI_CLASS] == ELFCLASS64) {
		Elf64_Ehdr elfEhdr;
		fread(&elfEhdr, 1, sizeof(elfEhdr), fp);
		if(opt & OPT_ELF_HEADER)
			print_ELF64_Header(elfEhdr);
		if(opt & OPT_SEC_HEADER)
			print_Section64_Header(fp, elfEhdr);
		if(opt & OPT_PRO_HEADER)
			print_Program64_Header(fp, elfEhdr);
	}
	else {
		printf("Error! ELF Class is invalid\n");
		exit(EXIT_FAILURE);
	}
	fclose(fp);
	exit(EXIT_SUCCESS);
}

void print_ELF32_Header(Elf32_Ehdr ehdr)
{
	printf("ELF Header:\n");

	printf(" Magic:   %c %c", ehdr.e_ident[EI_MAG0], ehdr.e_ident[EI_MAG1]);
	printf(" %c %c\n", ehdr.e_ident[EI_MAG2], ehdr.e_ident[EI_MAG3]);
    	// 1. Magic

	printf(" Class: \t\t\tELF32\n");
	// 2. Class

	printf(" Data: ");
	switch(ehdr.e_ident[EI_DATA]) {
        case ELFDATA2LSB:
		printf("\t\t\t\t2's complement, little endian\n");
		break;
        case ELFDATA2MSB:
		printf("\t\t\t\t2's complement, big endian\n");
		break;
        default:
		printf("\t\t\t\tInvalid data encoding\n");
	}
	// 3. Data

	printf(" Version: ");
	switch(ehdr.e_ident[EI_VERSION]){
	case EV_NONE:
		printf("\t\t\tInvalid version\n");
		break;
	case EV_CURRENT:
		printf("\t\t\tCurrent version\n");
		break;
	}
	// 4. Version

	printf(" OS/ABI :");
	switch(ehdr.e_ident[EI_OSABI]){
	case ELFOSABI_NONE:
		printf("\t\t\tNo extension or unspecified\n");
		break;
	case ELFOSABI_HPUX:
		printf("\t\t\tHewlett-Packard HP-UX\n");
		break;
	case ELFOSABI_NETBSD:
		printf("\t\t\tNetBSD\n");
		break;
	case ELFOSABI_LINUX:
		printf("\t\t\tLinux\n");
		break;
	case ELFOSABI_SOLARIS:
		printf("\t\t\tSun Solaris\n");
		break;
	case ELFOSABI_AIX:
		printf("\t\t\tAIX\n");
		break;
	case ELFOSABI_IRIX:
		printf("\t\t\tIRIX\n");
		break;
	case ELFOSABI_FREEBSD:
		printf("\t\t\tFreeBSD\n");
		break;
	case ELFOSABI_TRU64:
		printf("\t\t\tCompaq TRU64 UNIX\n");
		break;
	case ELFOSABI_MODESTO:
		printf("\t\t\tNovell Modesto\n");
		break;
	case ELFOSABI_OPENBSD:
		printf("\t\t\tOpen BSD\n");
		break;
	}
	// 5. OS/ABI

	printf(" ABI Version: ");
	switch(ehdr.e_ident[EI_ABIVERSION]){
	case EV_NONE:
		printf("\t\t\tInvalid version\n");
		break;
	case EV_CURRENT:
		printf("\t\t\tCurrent version\n");
		break;
	}
	// 6. ABI Version


	printf(" Type: ");
	switch(ehdr.e_type) {
        case ET_NONE:
		printf("\t\t\t\tNo file type\n");
		break;
        case ET_REL:
		printf("\t\t\t\tRelocatable file\n");
		break;
        case ET_EXEC:
		printf("\t\t\t\tExecutable file\n");
		break;
        case ET_DYN:
		printf("\t\t\t\tShared object file\n");
		break;
        case ET_CORE:
		printf("\t\t\t\tCore file\n");
		break;
        default:
		printf("\t\t\t\tUnknown (0x%x)\n", ehdr.e_type);
	}
	// 7. Type

	printf(" Machine: ");
	switch(ehdr.e_machine) {
        case EM_NONE:
		printf("\t\t\t\tNo machine\n");
		break;
        case EM_M32:
		printf("\t\t\t\tAt&T WE 32100\n");
		break;
        case EM_SPARC:
		printf("\t\t\t\tSPARC\n");
		break;
        case EM_386:
		printf("\t\t\t\tIntel 80386\n");
		break;
        case EM_68K:
		printf("\t\t\t\tMotorola 68000\n");
		break;
        case EM_88K:
		printf("\t\t\t\tMotorola 88000\n");
		break;
        case EM_860:
		printf("\t\t\t\tIntel 80860\n");
		break;
        case EM_MIPS:
		printf("\t\t\t\tMIPS RS3000\n");
		break;
        default:
		printf("\t\t\tUnknown (0x%x)\n", ehdr.e_machine);
	}
	// 8. Machine


	printf(" Version: ");
	switch(ehdr.e_version) {
        case EV_NONE:
		printf("\t\t\tInvalid version\n");
		break;
        case EV_CURRENT:
		printf("\t\t\tCurrent version\n");
		break;
        default:
		printf("\t\t\t0x%x\n", ehdr.e_version);
	}
	// 9. Version

	printf(" Entry point address: \t\t(0x%x)\n", ehdr.e_entry);
	// 10. Entry point address
	printf(" Start of program headers: \t%d (bytes into files)\n", ehdr.e_phoff);
	// 11. Start of program headers
	printf(" Start of section headers: \t%d (bytes into files)\n", ehdr.e_shoff);
	// 12. Start of Section headers	

	printf(" Flags: \t\t\t0x%x\n", ehdr.e_flags);
	// 13. Flags

	printf(" Size of this header: \t\t%d (bytes)\n", ehdr.e_ehsize);
	// 14. Size of this header
	printf(" Size of program headers: \t\t%d (bytes)\n", ehdr.e_phentsize);
	// 15. Size of program headers
	printf(" Number of program headers: \t\t%d\n", ehdr.e_phnum);
	// 16. Start of program headers
	printf(" Size of section headers: \t\t%d (bytes)\n", ehdr.e_shentsize);
	// 17. Start of program headers
	printf(" Number of secton headers: \t\t%d\n", ehdr.e_shnum);
	// 18. Number of section headers
	printf(" Section header string table index: \t%d\n", ehdr.e_shstrndx); 
	// 19. Section header string table index
}

void print_Program32_Header(FILE *fp, Elf32_Ehdr ehdr)
{
	char program_type[16];
	Elf32_Phdr elfPhdr;

	fseek(fp, ehdr.e_phoff, SEEK_SET);
	printf("%-16s %-18s %-18s %-18s\n", "Type", "Offset", "VirtualAddr", "PhysicalAddr");
	printf("%25s %20s %13s %7s\n", "FileSize", "MemorySize", "Flags", "Align" );
	int flag = 0;
	int i = 0;
	while(1){
		fread(&elfPhdr, 1, ehdr.e_phentsize, fp);
		memset(program_type, 0, sizeof(program_type));
		switch(elfPhdr.p_type){
		case PT_NULL:
			strcpy(program_type, "NULL");
			break;
		case PT_LOAD:
			strcpy(program_type, "LOAD");
			break;
		case PT_DYNAMIC:
			strcpy(program_type, "DYNAMIC");
			break;
		case PT_INTERP:
			strcpy(program_type, "INTERP");
			break;
		case PT_NOTE:
			strcpy(program_type, "NOTE");
			break;
		case PT_SHLIB:
			strcpy(program_type, "SHLIB");
			break;
		case PT_PHDR:
			strcpy(program_type, "PHDR");
			break;
		case PT_TLS:
			strcpy(program_type, "TLS");
			break;
		case PT_LOOS:
			strcpy(program_type, "LOOS");
			break;
		case PT_HIOS:
			strcpy(program_type, "HIOS");
			break;
		case PT_LOPROC:
			strcpy(program_type, "LOPROC");
			break;
		case PT_HIPROC:
			strcpy(program_type, "HIPROC");
			break;
		case GNU_EH_FRAME:
			strcpy(program_type, "GNU_EH_FRAME");
			break;
		case GNU_STACK:
			strcpy(program_type, "GNU_STACK");
			break;
		case GNU_RELRO:
			strcpy(program_type, "GNU_RELRO");
			flag = 1;
			break;
		}
		printf("%-16s ", program_type);
		printf("0x%016x ", elfPhdr.p_offset);
		printf("0x%016x ", elfPhdr.p_vaddr);
		printf("0x%016x ", elfPhdr.p_paddr);
		printf("\n                 ");

		printf("0x%016x ", elfPhdr.p_filesz);
		printf("0x%016x ", elfPhdr.p_memsz);
		printf("%2x ", elfPhdr.p_flags);
		printf("%7x ", elfPhdr.p_align);
		printf("\n\n");
		if(++i == ehdr.e_shnum || flag == 1) {flag = 0; break;}
	}
}

void print_Section32_Header(FILE* fp, Elf32_Ehdr ehdr)
{
	char *string_table;
	char section_type[16];
	Elf32_Shdr elfShdr;
	// read string table

	fseek(fp, ehdr.e_shoff + ehdr.e_shstrndx * sizeof(elfShdr), SEEK_SET);
	fread(&elfShdr, 1, sizeof(elfShdr), fp);
	
	string_table = (char*)malloc(elfShdr.sh_size);
	fseek(fp, elfShdr.sh_offset, SEEK_SET);
	fread(string_table, 1, elfShdr.sh_size, fp);

	printf("[%-3s] %-16s %-18s %-18s %-10s\n", "NUM", "Name", "Type", "Address", "offset");
	printf("      %-18s %-18s %7s %-6s %-6s %-6s\n", "Size", "EntSize", "Flags", "Link", "Info", "Align");
	// read all section headers and print it

	int i=0;
	while(1){
		fseek(fp, ehdr.e_shoff + i*ehdr.e_shentsize, SEEK_SET);
		fread(&elfShdr, 1, ehdr.e_shentsize, fp);

		printf("[%-3d]", i);
		printf("%-16s ", (string_table+elfShdr.sh_name));
		memset(section_type, 0, sizeof(section_type));
		switch(elfShdr.sh_type){
		case SHT_NULL:
			strcpy(section_type, "NULL");
			break;
		case SHT_PROGBITS:
			strcpy(section_type, "PROGBITS");
			break;
		case SHT_SYMTAB:
			strcpy(section_type, "SYMTAB");
			break;
		case SHT_STRTAB:
			strcpy(section_type, "STRTAB");
			break;
		case SHT_RELA:
			strcpy(section_type, "RELA");
			break;
		case SHT_HASH:
			strcpy(section_type, "HASH");
			break;
		case SHT_DYNAMIC:
			strcpy(section_type, "DYNAMIC");
			break;
		case SHT_NOTE:
			strcpy(section_type, "NOTE");
			break;
		case SHT_NOBITS:
			strcpy(section_type, "NOBITS");
			break;
		case SHT_REL:
			strcpy(section_type, "REL");
			break;
		case SHT_SHLIB:
			strcpy(section_type, "SHLIB");
			break;
		case SHT_DYNSYM:
			strcpy(section_type, "DYNSYM");
			break;
		case SHT_INIT_ARRAY:
			strcpy(section_type, "INIT_ARRAY");
			break;
		case SHT_FINI_ARRAY:
			strcpy(section_type, "FINI_ARRAY");
			break;
		case SHT_PREINIT_ARRAY:
			strcpy(section_type, "PREINIT_ARRAY");
			break;
		case SHT_GROUP:
			strcpy(section_type, "GROUP");
			break;
		case SHT_SYMTAB_SHNDX:
			strcpy(section_type, "SYMTAB_SHNDX");
			break;
		case SHT_LOOS:
			strcpy(section_type, "LOOS");
			break;
		case SHT_HIOS:
			strcpy(section_type, "HIOS");
			break;
		case SHT_LOPROC:
			strcpy(section_type, "LOPROC");
			break;
		case SHT_HIPROC:
			strcpy(section_type, "HIPROC");
			break;
		case SHT_LOUSER:
			strcpy(section_type, "LOUSER");
			break;
		case SHT_HIUSER:
			strcpy(section_type, "HIUSER");
			break;
		}
		printf("%-18s ", section_type);
		printf("0x%016u ", elfShdr.sh_addr);
		printf("0x%08u ", elfShdr.sh_offset);
		printf("\n      ");

		printf("0x%016u ", elfShdr.sh_size);
		printf("0x%016u ", elfShdr.sh_entsize);
		printf("%-6u ", elfShdr.sh_flags);
		printf("%-6x ", elfShdr.sh_link);
		printf("%-6x ", elfShdr.sh_info);
		printf("%-6u ", elfShdr.sh_addralign);
		printf("\n");
		if(++i == ehdr.e_shnum) {
		printf("Key to Flags:\n");
		printf("  W (write), A (alloc), X (execute), M (merge),");
		printf(" S (strings), l (large)\n");
		printf("  I (info), L (link order), G (group), T (TLS),");
		printf(" E (exclude), x (unknown)\n");
		printf("  O (extra OS processing required) o (OS specific)");
		printf(",p (processor specific)\n");
		break;	}
	}
}




// 64-bit
void print_ELF64_Header(Elf64_Ehdr ehdr)
{
	printf("ELF Header:\n");

	printf(" Magic:   %c %c", ehdr.e_ident[EI_MAG0], ehdr.e_ident[EI_MAG1]);
	printf(" %c %c\n", ehdr.e_ident[EI_MAG2], ehdr.e_ident[EI_MAG3]);
    	// 1. Magic

	printf(" Class: \t\t\tELF64\n");
	// 2. Class

	printf(" Data: ");
	switch(ehdr.e_ident[EI_DATA]) {
        case ELFDATA2LSB:
		printf("\t\t\t\t2's complement, little endian\n");
		break;
        case ELFDATA2MSB:
		printf("\t\t\t\t2's complement, big endian\n");
		break;
        default:
		printf("\t\t\t\tInvalid data encoding\n");
	}
	// 3. Data

	printf(" Version: ");
	switch(ehdr.e_ident[EI_VERSION]){
	case EV_NONE:
		printf("\t\t\tInvalid version\n");
		break;
	case EV_CURRENT:
		printf("\t\t\tCurrent version\n");
		break;
	}
	// 4. Version

	printf(" OS/ABI :");
	switch(ehdr.e_ident[EI_OSABI]){
	case ELFOSABI_NONE:
		printf("\t\t\tNo extension or unspecified\n");
		break;
	case ELFOSABI_HPUX:
		printf("\t\t\tHewlett-Packard HP-UX\n");
		break;
	case ELFOSABI_NETBSD:
		printf("\t\t\tNetBSD\n");
		break;
	case ELFOSABI_LINUX:
		printf("\t\t\tLinux\n");
		break;
	case ELFOSABI_SOLARIS:
		printf("\t\t\tSun Solaris\n");
		break;
	case ELFOSABI_AIX:
		printf("\t\t\tAIX\n");
		break;
	case ELFOSABI_IRIX:
		printf("\t\t\tIRIX\n");
		break;
	case ELFOSABI_FREEBSD:
		printf("\t\t\tFreeBSD\n");
		break;
	case ELFOSABI_TRU64:
		printf("\t\t\tCompaq TRU64 UNIX\n");
		break;
	case ELFOSABI_MODESTO:
		printf("\t\t\tNovell Modesto\n");
		break;
	case ELFOSABI_OPENBSD:
		printf("\t\t\tOpen BSD\n");
		break;
	}
	// 5. OS/ABI

	printf(" ABI Version: ");
	switch(ehdr.e_ident[EI_ABIVERSION]){
	case EV_NONE:
		printf("\t\t\tInvalid version\n");
		break;
	case EV_CURRENT:
		printf("\t\t\tCurrent version\n");
		break;
	}
	// 6. ABI Version


	printf(" Type: ");
	switch(ehdr.e_type) {
        case ET_NONE:
		printf("\t\t\t\tNo file type\n");
		break;
        case ET_REL:
		printf("\t\t\t\tRelocatable file\n");
		break;
        case ET_EXEC:
		printf("\t\t\t\tExecutable file\n");
		break;
        case ET_DYN:
		printf("\t\t\t\tShared object file\n");
		break;
        case ET_CORE:
		printf("\t\t\t\tCore file\n");
		break;
        default:
		printf("\t\t\t\tUnknown (0x%x)\n", ehdr.e_type);
	}
	// 7. Type

	printf(" Machine: ");
	switch(ehdr.e_machine) {
        case EM_NONE:
		printf("\t\t\t\tNo machine\n");
		break;
        case EM_M32:
		printf("\t\t\t\tAt&T WE 32100\n");
		break;
        case EM_SPARC:
		printf("\t\t\t\tSPARC\n");
		break;
        case EM_386:
		printf("\t\t\t\tIntel 80386\n");
		break;
        case EM_68K:
		printf("\t\t\t\tMotorola 68000\n");
		break;
        case EM_88K:
		printf("\t\t\t\tMotorola 88000\n");
		break;
        case EM_860:
		printf("\t\t\t\tIntel 80860\n");
		break;
        case EM_MIPS:
		printf("\t\t\t\tMIPS RS3000\n");
		break;
        default:
		printf("\t\t\tUnknown (0x%x)\n", ehdr.e_machine);
	}
	// 8. Machine


	printf(" Version: ");
	switch(ehdr.e_version) {
        case EV_NONE:
		printf("\t\t\tInvalid version\n");
		break;
        case EV_CURRENT:
		printf("\t\t\tCurrent version\n");
		break;
        default:
		printf("\t\t\t0x%x\n", ehdr.e_version);
	}
	// 9. Version

	printf(" Entry point address: \t\t(0x%lx)\n", ehdr.e_entry);
	// 10. Entry point address
	printf(" Start of program headers: \t%ld (bytes into files)\n", ehdr.e_phoff);
	// 11. Start of program headers
	printf(" Start of section headers: \t%ld (bytes into files)\n", ehdr.e_shoff);
	// 12. Start of Section headers	

	printf(" Flags: \t\t\t0x%x\n", ehdr.e_flags);
	// 13. Flags

	printf(" Size of this header: \t\t%d (bytes)\n", ehdr.e_ehsize);
	// 14. Size of this header
	printf(" Size of program headers: \t\t%d (bytes)\n", ehdr.e_phentsize);
	// 15. Size of program headers
	printf(" Number of program headers: \t\t%d\n", ehdr.e_phnum);
	// 16. Start of program headers
	printf(" Size of section headers: \t\t%d (bytes)\n", ehdr.e_shentsize);
	// 17. Start of program headers
	printf(" Number of secton headers: \t\t%d\n", ehdr.e_shnum);
	// 18. Number of section headers
	printf(" Section header string table index: \t%d\n", ehdr.e_shstrndx); 
	// 19. Section header string table index
}

void print_Program64_Header(FILE *fp, Elf64_Ehdr ehdr)
{
	char program_type[16];
	Elf64_Phdr elfPhdr;

	fseek(fp, ehdr.e_phoff, SEEK_SET);
	printf("%-16s %-18s %-18s %-18s\n", "Type", "Offset", "VirtualAddr", "PhysicalAddr");
	printf("%25s %20s %13s %7s\n", "FileSize", "MemorySize", "Flags", "Align" );
	int flag = 0;
	int i = 0;
	while(1){
		fread(&elfPhdr, 1, ehdr.e_phentsize, fp);
		memset(program_type, 0, sizeof(program_type));
		switch(elfPhdr.p_type){
		case PT_NULL:
			strcpy(program_type, "NULL");
			break;
		case PT_LOAD:
			strcpy(program_type, "LOAD");
			break;
		case PT_DYNAMIC:
			strcpy(program_type, "DYNAMIC");
			break;
		case PT_INTERP:
			strcpy(program_type, "INTERP");
			break;
		case PT_NOTE:
			strcpy(program_type, "NOTE");
			break;
		case PT_SHLIB:
			strcpy(program_type, "SHLIB");
			break;
		case PT_PHDR:
			strcpy(program_type, "PHDR");
			break;
		case PT_TLS:
			strcpy(program_type, "TLS");
			break;
		case PT_LOOS:
			strcpy(program_type, "LOOS");
			break;
		case PT_HIOS:
			strcpy(program_type, "HIOS");
			break;
		case PT_LOPROC:
			strcpy(program_type, "LOPROC");
			break;
		case PT_HIPROC:
			strcpy(program_type, "HIPROC");
			break;
		case GNU_EH_FRAME:
			strcpy(program_type, "GNU_EH_FRAME");
			break;
		case GNU_STACK:
			strcpy(program_type, "GNU_STACK");
			break;
		case GNU_RELRO:
			strcpy(program_type, "GNU_RELRO");
			flag = 1;
			break;
		}
		printf("%-16s ", program_type);
		printf("0x%016lx ", elfPhdr.p_offset);
		printf("0x%016lx ", elfPhdr.p_vaddr);
		printf("0x%016lx ", elfPhdr.p_paddr);
		printf("\n                 ");

		printf("0x%016lx ", elfPhdr.p_filesz);
		printf("0x%016lx ", elfPhdr.p_memsz);
		printf("%2x ", elfPhdr.p_flags);
		printf("%7lx ", elfPhdr.p_align);
		printf("\n\n");
		if(++i == ehdr.e_shnum || flag == 1) {flag = 0; break;}
	}
}
void print_Section64_Header(FILE* fp, Elf64_Ehdr ehdr)
{
	char *string_table;
	char section_type[16];
	Elf64_Shdr elfShdr;
	// read string table

	fseek(fp, ehdr.e_shoff + ehdr.e_shstrndx * sizeof(elfShdr), SEEK_SET);
	fread(&elfShdr, 1, sizeof(elfShdr), fp);
	
	string_table = (char*)malloc(elfShdr.sh_size);
	fseek(fp, elfShdr.sh_offset, SEEK_SET);
	fread(string_table, 1, elfShdr.sh_size, fp);

	printf(" [Nr] %-18s %-18s %-18s %-10s\n", "Name", "Type", "Address", "offset");
	printf("      %-18s %-18s %5s %-6s %-6s %-6s\n", "Size", "EntSize", "Flags", "Link", "Info", "Align");
	// read all section headers and print it

	int i=0;
	while(1){
		fseek(fp, ehdr.e_shoff + i*ehdr.e_shentsize, SEEK_SET);
		fread(&elfShdr, 1, ehdr.e_shentsize, fp);

		printf(" [%-3d]", i);
		printf("%-18s ", (string_table+elfShdr.sh_name));
		memset(section_type, 0, sizeof(section_type));
		switch(elfShdr.sh_type){
		case SHT_NULL:
			strcpy(section_type, "NULL");
			break;
		case SHT_PROGBITS:
			strcpy(section_type, "PROGBITS");
			break;
		case SHT_SYMTAB:
			strcpy(section_type, "SYMTAB");
			break;
		case SHT_STRTAB:
			strcpy(section_type, "STRTAB");
			break;
		case SHT_RELA:
			strcpy(section_type, "RELA");
			break;
		case SHT_HASH:
			strcpy(section_type, "HASH");
			break;
		case SHT_DYNAMIC:
			strcpy(section_type, "DYNAMIC");
			break;
		case SHT_NOTE:
			strcpy(section_type, "NOTE");
			break;
		case SHT_NOBITS:
			strcpy(section_type, "NOBITS");
			break;
		case SHT_REL:
			strcpy(section_type, "REL");
			break;
		case SHT_SHLIB:
			strcpy(section_type, "SHLIB");
			break;
		case SHT_DYNSYM:
			strcpy(section_type, "DYNSYM");
			break;
		case SHT_INIT_ARRAY:
			strcpy(section_type, "INIT_ARRAY");
			break;
		case SHT_FINI_ARRAY:
			strcpy(section_type, "FINI_ARRAY");
			break;
		case SHT_PREINIT_ARRAY:
			strcpy(section_type, "PREINIT_ARRAY");
			break;
		case SHT_GROUP:
			strcpy(section_type, "GROUP");
			break;
		case SHT_SYMTAB_SHNDX:
			strcpy(section_type, "SYMTAB_SHNDX");
			break;
		case SHT_LOOS:
			strcpy(section_type, "LOOS");
			break;
		case SHT_HIOS:
			strcpy(section_type, "HIOS");
			break;
		case SHT_LOPROC:
			strcpy(section_type, "LOPROC");
			break;
		case SHT_HIPROC:
			strcpy(section_type, "HIPROC");
			break;
		case SHT_LOUSER:
			strcpy(section_type, "LOUSER");
			break;
		case SHT_HIUSER:
			strcpy(section_type, "HIUSER");
			break;
		}
		printf("%-18s ", section_type);
		printf("0x%016lu ", elfShdr.sh_addr);
		printf("0x%08lu ", elfShdr.sh_offset);
		printf("\n      ");

		printf("0x%016lu ", elfShdr.sh_size);
		printf("0x%016lu ", elfShdr.sh_entsize);
		printf("%-6lu ", elfShdr.sh_flags);
		printf("%-6x ", elfShdr.sh_link);
		printf("%-6x ", elfShdr.sh_info);
		printf("%-6lu ", elfShdr.sh_addralign);
		printf("\n");
		if(++i == ehdr.e_shnum) {
		printf("Key to Flags:\n");
		printf("  W (write), A (alloc), X (execute), M (merge),");
		printf(" S (strings), l (large)\n");
		printf("  I (info), L (link order), G (group), T (TLS),");
		printf(" E (exclude), x (unknown)\n");
		printf("  O (extra OS processing required) o (OS specific)");
		printf(",p (processor specific)\n");
		break;}
	}
}


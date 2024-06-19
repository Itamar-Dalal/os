

int kmain(void *mbd, unsigned int magic) {
	if (magic != 0x2BADB002) {
		return -1;
	}
	return 0;
}
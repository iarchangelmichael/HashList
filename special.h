#define tbtime timeb ft, fte; int tbtime_sec, tbtime_millim, tbtime_result; ftime(&ft); // print("Time: ", itos(sec*1000+millitm), "ms.\r\n");
#define tctime ftime(&ft); // continue
#define tetime ftime(&fte); tbtime_sec=(int)(fte.time-ft.time); tbtime_millim=fte.millitm-ft.millitm; if(fte.millitm<ft.millitm){ tbtime_millim+=1000; tbtime_sec--; } tbtime_result = tbtime_sec * 1000 + tbtime_millim;


unsigned int crc32(unsigned char *buf, unsigned long len){
	unsigned long crc_table[256];
	unsigned long crc;
	for (int i = 0; i < 256; i++){
		crc = i;
		for (int j = 0; j < 8; j++)
		crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
		crc_table[i] = crc;
	};
	crc = 0xFFFFFFFFUL;
	while (len--)
		crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
	return crc ^ 0xFFFFFFFFUL;
}

#define OMatrixTemplateAdd(_a, _e, el) \
	if(!_a){ _a=el; _e=el; el->_p=0; el->_n=0; } \
	else {	el->_n=0; el->_p=_e; _e->_n=el; _e=el; }

#define OMatrixTemplateDel(_a, _e, el) \
	if(el->_n) el->_n->_p=el->_p; else if(el==_e) _e=el->_p; \
	if(el->_p) el->_p->_n=el->_n; else if(el==_a) _a=el->_n;


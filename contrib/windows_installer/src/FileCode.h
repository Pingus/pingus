

bool Exists(char* File);
bool ExistsDir(char* File);
bool CanReadWrite(char* File);
void NormalPath(char* File);
bool EnsureFolder(char* File);
void FileSize(__int64 Size, char* Buffer);

const int MaxFileSizeBuf = 10;

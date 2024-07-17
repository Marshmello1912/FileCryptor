#include<iostream>

using namespace std;

long get_num_file(int i, long* arr) {
	srand(i * 20);
	while (true) {	
		long tmp = rand();
		bool flag = true;
		for (int j = 0; j < i; j++) {
			if (tmp == arr[j]) {
				flag = false;
				break;
			}
		}
		if (flag) {
			arr[i] = tmp;
			return tmp;
		}
	}
}

char* get_file_name(int file_num, const char* file_extension, const char* FilePath) {
	int i = file_num;
	int k = 0;
	int chars_in_file_extension = 0;
	int chars_in_file_path = 0;

	for (;;) {
		if (*(file_extension + chars_in_file_extension) == '\0') {
			break;
		}
		chars_in_file_extension++;
	}
	for (;;) {
		if (*(FilePath + chars_in_file_path) == '\0') {
			break;
		}
		chars_in_file_path++;
	}

	if (file_num < 10 and file_num >= 0) {
		k = 1;
	}
	else {
		while (i != 0) {
			i = i / 10;
			k++;
		}
	}

	char* num = new char[k];
	for (int i = k - 1; i >= 0; i--) {
		num[i] = (file_num % 10) + 48;
		file_num /= 10;
	}

	char* file_name = new char[k + chars_in_file_extension + chars_in_file_path + 1];
	file_name[k + chars_in_file_extension + chars_in_file_path] = '\0';

	int j = 0;
	while (j < chars_in_file_path) {
		file_name[j] = FilePath[j];
		j++;
	}

	for (int i = chars_in_file_path; i < k + chars_in_file_path; i++) {
		file_name[i] = num[i - chars_in_file_path];
	}
	for (int i = k + chars_in_file_path; i < k + chars_in_file_path + chars_in_file_extension; i++) {
		file_name[i] = file_extension[i - k - chars_in_file_path];
	}

	return file_name;
}



//������� ���������� �����
void split(const char* FileName, const char* filesExtensions, const char* PathToFiles = "") {
	FILE* pFile;
	FILE* pFile2;
	char* buffer;
	char* BufferWithTrash;
	char* buffer2;
	long lSize1;


	//�������� ������ ��� ��������� �����
	fopen_s(&pFile, FileName, "rb");

	//������ ������� ��������� ����� 
	fseek(pFile, 0, SEEK_END);
	lSize1 = ftell(pFile);
	rewind(pFile);

	// ������ ������ �����
	long int Split_size = 1024 * 300;

	//������ ���������� ������
	long int iterator = lSize1 / Split_size;

	//��������� ������ ��� ����� ��� �������� ������ � ��������������� �����
	buffer = (char*)malloc(Split_size);
	BufferWithTrash = (char*)malloc(Split_size * 2);
	buffer2 = (char*)malloc((lSize1 % Split_size) * 2);
	long* nums = new long[iterator + 1];
	//���� �������� � ���������� �������� ������
	for (long int i = 0; i < iterator; i++) {
		//�������� ����� ��� ���������� ����� ������
		fopen_s(&pFile2, get_file_name(get_num_file(i, nums), filesExtensions, PathToFiles), "wb");
		srand(time(NULL));
	
		//������ � ������ ������
		fread(buffer, 1, Split_size, pFile);
		for (int j = 0; j < Split_size; j++) {
			BufferWithTrash[j] = rand() % 255;
			BufferWithTrash[j + Split_size] = buffer[j];
		}
		fwrite(BufferWithTrash, 1, Split_size * 2, pFile2);
		fclose(pFile2);
	}
	//�������� � ���������� ��������������� ����� � ������ ���� ������ ��������� ����� �� ������ Split_size
	if (lSize1 % Split_size != 0) {
		BufferWithTrash = (char*)malloc((lSize1 % Split_size) * 2);
		//�������� ����� ��� ���������� ����� ������
		fopen_s(&pFile2, get_file_name(get_num_file(iterator, nums), filesExtensions, PathToFiles), "wb");
		
		//������ � ������ ������
		fread(buffer2, 1, lSize1 % Split_size, pFile);
		for (int j = 0; j < lSize1 % Split_size; j++) {
			BufferWithTrash[j] = rand() % 255;
			BufferWithTrash[j + (lSize1 % Split_size)] = buffer2[j];
		}
		fwrite(buffer2, 1, (lSize1 % Split_size) * 2, pFile2);
		fclose(pFile2);
	}

}

//������� ����������� ������
void merge(const char* FileName, const char* filesExtension, const char* PathToFiles = "") {
	FILE* pFile;
	FILE* pFile2;
	char* buffer;
	long lSize1;
	//�������� ������ ��� ��������� �����
	fopen_s(&pFile, FileName, "wb");
	printf("������� ���-�� ������: ");
	//���� ���������� ������
	int Index_Of_End;
	cin >> Index_Of_End;
	Index_Of_End--;
	long* nums = new long[Index_Of_End];
	//���� ������ ������
	for (int i = 0; i <= Index_Of_End; i++) {
		//�������� ����� � ������ ������
		fopen_s(&pFile2, get_file_name(get_num_file(i, nums), filesExtension, PathToFiles), "rb");

		//������ ������� �����
		fseek(pFile2, 0, SEEK_END);
		lSize1 = ftell(pFile2) / 2;
		rewind(pFile2);
		fseek(pFile2, lSize1, SEEK_SET);
		//��������� ������ ��� ������
		buffer = (char*)malloc(lSize1);

		//������ � ������ ������
		fread(buffer, 1, lSize1, pFile2);
		fwrite(buffer, 1, lSize1, pFile);

		fclose(pFile2);

	}
	fclose(pFile);
}

void main() {

	setlocale(LC_ALL, "RU");
	split("Video.mp4", ".dat", "data/");
	merge("Video2.mp4", ".dat", "data/");
}
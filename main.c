#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int id;
    char name[50];
    float grade;
};

int idExists(int id) {
    FILE *fp = fopen("students.dat", "rb");
    if (fp == NULL) return 0;

    struct Student s;
    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id == id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void addStudent() {
    struct Student s;

    printf("ID: ");
    scanf("%d", &s.id);

    if (idExists(s.id)) {
        printf("Bu ID zaten var!\n");
        return;
    }

    printf("Name: ");
    scanf(" %[^\n]", s.name);

    printf("Grade: ");
    scanf("%f", &s.grade);

    FILE *fp = fopen("students.dat", "ab");
    if (fp == NULL) {
        printf("Dosya acilamadi!\n");
        return;
    }

    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);

    printf("Ogrenci eklendi!\n");
}

void listStudents() {
    FILE *fp = fopen("students.dat", "rb");

    if (fp == NULL) {
        printf("Henuz kayit yok!\n");
        return;
    }

    struct Student s;

    printf("\n%-5s %-15s %-5s\n", "ID", "Name", "Grade");
    printf("-----------------------------\n");

    while (fread(&s, sizeof(s), 1, fp)) {
        printf("%-5d %-15s %-5.2f\n", s.id, s.name, s.grade);
    }

    fclose(fp);
}

void searchStudent() {
    FILE *fp = fopen("students.dat", "rb");

    if (fp == NULL) {
        printf("Henuz kayit yok!\n");
        return;
    }

    struct Student s;
    int id, found = 0;

    printf("Aranacak ID: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id == id) {
            printf("Bulundu -> %s %.2f\n", s.name, s.grade);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Ogrenci bulunamadi!\n");

    fclose(fp);
}

void deleteStudent() {
    FILE *fp = fopen("students.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (fp == NULL || temp == NULL) {
        printf("Dosya hatasi!\n");
        return;
    }

    struct Student s;
    int id, found = 0;

    printf("Silinecek ID: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id != id) {
            fwrite(&s, sizeof(s), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found)
        printf("Silme islemi tamamlandi!\n");
    else
        printf("Ogrenci bulunamadi!\n");
}

void updateStudent() {
    FILE *fp = fopen("students.dat", "rb+");

    if (fp == NULL) {
        printf("Henuz kayit yok!\n");
        return;
    }

    struct Student s;
    int id, found = 0;

    printf("Guncellenecek ID: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id == id) {
            printf("Yeni isim: ");
            scanf(" %[^\n]", s.name);

            printf("Yeni not: ");
            scanf("%f", &s.grade);

            fseek(fp, -sizeof(s), SEEK_CUR);
            fwrite(&s, sizeof(s), 1, fp);

            printf("Guncellendi!\n");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Ogrenci bulunamadi!\n");

    fclose(fp);
}

void calculateAverage() {
    FILE *fp = fopen("students.dat", "rb");

    if (fp == NULL) {
        printf("Henuz kayit yok!\n");
        return;
    }

    struct Student s;
    float total = 0;
    int count = 0;

    while (fread(&s, sizeof(s), 1, fp)) {
        total += s.grade;
        count++;
    }

    if (count == 0)
        printf("Kayit yok!\n");
    else
        printf("Ortalama: %.2f\n", total / count);

    fclose(fp);
}

void menu() {
    int choice;

    while (1) {
        printf("\n===== MENU =====\n");
        printf("1. Ogrenci Ekle\n");
        printf("2. Listele\n");
        printf("3. Ara\n");
        printf("4. Sil\n");
        printf("5. Guncelle\n");
        printf("6. Ortalama\n");
        printf("7. Cikis\n");
        printf("Secim: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: listStudents(); break;
            case 3: searchStudent(); break;
            case 4: deleteStudent(); break;
            case 5: updateStudent(); break;
            case 6: calculateAverage(); break;
            case 7: return;
            default: printf("Hatali secim!\n");
        }
    }
}

int main() {
    menu();
    return 0;
}

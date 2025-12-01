#include<stdio.h>
#include<string.h>
#define MAX_BOOKS 100
#define MAX_BORROWS 100
#define ITEMS_PER_PAGE 2

struct Date{
    int day;
    int month;
    int year; 
};
struct Book{
    int bookId;
    char title[50];
    char author[50];    
    int publishYear;
    int quantity;
};
struct Borrow{
    int borrowId;
    int bookId;
    struct Date borrowDate;
    struct Date borrowReturn;
    char borrowerName[50];
    int status;
};

struct Book books[MAX_BOOKS];
struct Borrow borrows[MAX_BORROWS];
int bookCount = 0;
int borrowCount = 0; 
int nextBookId = 100;
int nextBorrowId = 2000;

// kiem tra nam nhuan
int isLeapYear(int year){
    if((year % 400 ==0)||(year % 4 ==0 && year %100 != 0)){
        return 1;
    }
    return 0;
}
int getDaysInMonth(int month, int year){
    if(month == 2){
        return isLeapYear(year) ? 29 : 28;
    }
    else if(month == 4 || month ==6 || month ==9 || month == 11){
        return 30;
    }
    else if(month >= 1 && month <= 12){
        return 31;
    }
    return 0;
}

int findBookIndexById(int id){
    for (int i = 0; i < bookCount; i++){
        if(books[i].bookId == id){
            return i;
        }
    }
    return -1;
}

int findBorrowIndexById(int id){
    for(int i = 0; i < borrowCount; i++){
        if (borrows[i].borrowId == id){
            return i;
        }
    }
    return -1;
}
// Nam, so luong, id
int getIntInput(const char *prompt, int minVal, int maxVal){
    int inValue;
    char buffer[100];
    while(1){
        printf("%s",prompt);
        if(fgets(buffer, sizeof(buffer), stdin) != NULL){
            char extra;
            // sscanf tra ve 1 neu dung so nguyen , 2 neu con ky tu khac
            if(sscanf(buffer, "%d %c", &inValue, &extra) == 1){
                if(inValue >= minVal && inValue <= maxVal){
                    return inValue;
                } else {
                    printf("Gia tri phai tu %d den %d. Nhap lai.\n", minVal, maxVal);
                }
            } else {
                printf("Chi nhap so nguyen hop le. Nhap lai.\n");
            }
        } else {
            printf("Loi: Dau vao khong hop le.\n");
        }
    }
}
//ten sach, tac gia
void getStringInput(const char *prompt, char *buffer, size_t size){
    while (1){
        printf("%s", prompt);
        if(fgets(buffer, size,stdin) != NULL){
            buffer[strcspn(buffer,"\n")] = 0;
            
            int onlySpace = 1;
            for (int i = 0; buffer[i] != 0; i++){
                if (buffer[i] != ' ') {
                    onlySpace = 0;
                    break; 
                }
            }
            if(strlen(buffer) > 0 && !onlySpace ){
                return;
            }
        }
        printf("Loi: Thong tin khong duoc de trong.\n");
    }
}
//goi ham de neu trung thi se bat nhap lai
int isTitleUnique(const char *title, int ignoreId){
    for(int i = 0; i < bookCount; i++){
        if(strcmp(books[i].title, title) == 0 && books[i].bookId != ignoreId){
            return 0;
        }
    }
    return 1;
}
// kiem tra k dc chua so
int hasDigit(const char *s){
    for(int i = 0; s[i] != '\0'; i++){
        if(s[i] >= '0' && s[i] <= '9'){
            return 1;
        }
    }
    return 0;
} 

void strToLower(char *dest,const char *src){
	int i = 0;
	while(src[i]){
		if(src[i] >= 'A' && src[i] <= 'Z'){
			dest[i] = src[i] +32; //chuyen sang chu thuog
		}else{
			dest[i] = src[i];
		}
		i++;
	}
	dest[i] = '\0';
}

void addBook(){
    if (bookCount >= MAX_BOOKS){
        printf("Danh sach da day, khong the them moi\n");
        return;
    } 

    struct Book newBook;
    newBook.bookId = nextBookId++; // gan
    printf("\n======= Them moi sach =====\n");
    printf("Book ID tu dong: %d\n",newBook.bookId);

    while(1){
        getStringInput("Nhap tieu de sach (title): ", newBook.title, sizeof(newBook.title));
        if (isTitleUnique(newBook.title, 0)){
            break;
        }
        printf("Loi: Tieu de sach da ton tai. Nhap lai.\n");
    }
    newBook.publishYear = getIntInput("Nhap nam xuat ban (>1900): ", 1901,9999);
    while (1) {
        getStringInput("Nhap ten tac gia moi: ", newBook.author, sizeof(newBook.author));
        if (!hasDigit(newBook.author)) break;
        printf("Loi: Ten tac gia khong duoc chua so!\n");
    }
    newBook.quantity = getIntInput("Nhap so luong: ", 1,100);
    books[bookCount++] = newBook;
    printf("Them sach '%s' (ID: %d) thanh cong\n", newBook.title, newBook.bookId);
}


void updateBook(){
    printf("\n======= Cap nhat thong tin sach =====\n");
    if (bookCount == 0){
        printf("Danh sach sach trong, khong co gi de cap nhat\n");
        return;
    }
    int idToUpdate = getIntInput("Nhap Book ID can cap nhat: ", 100,100+MAX_BOOKS-1);
    int index = findBookIndexById(idToUpdate);
    if(index == -1){
        printf("Loi: ID sach %d khong ton tai\n", idToUpdate);
        return;
    }
    struct Book *currentBook = &books[index];
    char newTitle[100], newAuthor[50];
    printf("Dang cap nhat sach ID: %d, Tieu de cu: %s\n", currentBook->bookId, currentBook->title);
    printf("bookId: %d\n", currentBook->bookId);
    while(1){
        getStringInput("Nhap tieu de moi: ", newTitle, sizeof(newTitle));
        if(isTitleUnique(newTitle, currentBook->bookId)){
            break;
        }
        printf("Loi: Tieu de sach da ton tai. Nhap lai.\n");
    }
    currentBook->quantity = getIntInput("Nhap so luong moi: ", 1,100);
    currentBook->publishYear = getIntInput("Nhap nam xuat ban moi: ", 1901,9999);
    while(1){
        getStringInput("Nhap ten tac gia moi: ", newAuthor, sizeof(newAuthor));
        if(!hasDigit(newAuthor)){
            break;
        }
        printf("Loi: Ten tac gia khong duoc chua so\n");
    }
    strcpy(currentBook->title, newTitle);
    strcpy(currentBook->author, newAuthor);
    printf("Cap nhat sach ID %d thanh cong\n", currentBook->bookId);
}

void displayBooks(){
    if(bookCount == 0){
        printf("Danh sach trong\n");
        return; 
    } 
    int page = 1;
    int total_page;
    int kq = bookCount / ITEMS_PER_PAGE;
    
    total_page = (bookCount % ITEMS_PER_PAGE == 0) ? kq : kq + 1;
    while(1){
        int start = (page -1) * ITEMS_PER_PAGE;
        int end = start + ITEMS_PER_PAGE;
        
        printf("\n======== Trang %d/%d ========\n", page, total_page);
        printf("+-----+------------------------------+--------------------+---------+------+\n");
        printf("|%-5s|%-30s|%-20s|%-7s|%-8s|\n","ID","Tieu de","Tac gia","Nam","SL");
        printf("+-----+------------------------------+--------------------+-------+--------+\n");
        
        for(int i = start; i < end && i<bookCount; i++){
            printf("|%-5d|%-30s|%-20s|%-7d|%-8d|\n",books[i].bookId,books[i].title,books[i].author,books[i].publishYear,books[i].quantity);
        }
        printf("+-----+------------------------------+--------------------+-------+--------+\n");
        int choicePage;
        do{
            printf("1.Trang truoc\n");
            printf("2.Thoat\n");
            printf("3.Trang sau\n");
            choicePage = getIntInput("Nhap lua chon (1-3): ",1,3);
            
            if(choicePage < 1 || choicePage > 3){
                printf("Lua chon khong hop le, nhap lai\n");
            }
        }while(choicePage < 1 || choicePage > 3);
        
        if (choicePage == 2){
            return;
        }
        if(choicePage == 1){
            if(page > 1){
                page--;
            }
            continue;
        }
        if (choicePage == 3){
            if(page < total_page){
                page++;
            }
            continue;
        }
    }
}
void deleteBook(){
    if(bookCount == 0){
        printf("Danh sach trong, khong co gi de xoa\n");
        return;
    } 
    int id = getIntInput("Nhap Book ID can xoa: ",100,100+MAX_BOOKS-1);
    int index = findBookIndexById(id);
    if (index == -1){
        printf("ID sach %d khogn ton tai\n",id);
        return;
    }
    for (int i = 0;  i < borrowCount; i++){
        if(borrows[i].bookId == id && borrows[i].status == 1){
            printf("Khong the xoa sach vi co nguoi dang muon\n");
            return;
        }
    }
    for (int i = index; i < bookCount -1; i++){
        books[i] = books[i + 1];
    }
    bookCount--;
    printf("Xoa sach ID %d thanh cong\n ", id);
}
void searchBook(){
    if(bookCount == 0){
        printf("Danh sach trong\n");
        return;
    }
    char searchTitle[50];
    getStringInput("Nhap ten sach can tim: ", searchTitle,sizeof(searchTitle));
    
    char searchLower[50];
    strToLower(searchLower, searchTitle); 
    
    printf("\n Ket qua tim kiem: \n");
    printf("+-----+------------------------------+--------------------+---------+------+\n");
    printf("|%-5s|%-30s|%-20s|%-7s|%-8s|\n", "ID", "Tieu de", "Tac gia", "Nam", "SL");
    printf("+-----+------------------------------+--------------------+-------+--------+\n");
    
    int found = 0;
    for(int i = 0; i< bookCount; i++){
    	char titleLower[50];
    	strToLower(titleLower, books[i].title);
        if (strstr(titleLower,searchLower) != NULL){
            printf("|%-5d|%-30s|%-20s|%-7d|%-8d|\n", books[i].bookId,books[i].title,books[i].author,books[i].publishYear,books[i].quantity);
            found = 1;
        }
    }
    if (!found){
        printf("Khong tim thay sach nao voi ten '%s'\n",searchTitle);
    }
    printf("+-----+------------------------------+--------------------+-------+--------+\n");
}
void addBorrowSlip(){
    if(borrowCount >= MAX_BORROWS){
        printf("Danh sach phieu muon da day, khong the them moi\n");
        return;
    }
    struct Borrow newBorrow;
    newBorrow.borrowId = nextBorrowId++;
    printf("\n======== Them moi phieu muon ======\n");
    printf("Borrow ID tu dong: %d\n", newBorrow.borrowId);
    
    while(1){
        newBorrow.bookId = getIntInput("Nhap Book ID can muon: ",100,100+MAX_BOOKS-1);
        int index = findBookIndexById(newBorrow.bookId);
        if(index == -1){
            printf("Loi: Book ID khong ton tai.Vui long nhap lai\n");
        }
        else if(books[index].quantity <= 0){
            printf("Sach da muon, khong the muon them.Nhap ID khac\n");
        }
        else{
            break;
        }
    }

    while(1){
        newBorrow.borrowDate.day = getIntInput("Nhap ngay muon: ",1,31);
        newBorrow.borrowDate.month = getIntInput("Nhap thang muon: ",1,12);
        newBorrow.borrowDate.year = getIntInput("Nhap nam muon: ",1900,9999);

        int maxDay = getDaysInMonth(newBorrow.borrowDate.month, newBorrow.borrowDate.year);
        if(newBorrow.borrowDate.day <= maxDay){
            break;
        }
        printf("Ngay muon khong hop le. Vui long nhap lai\n");
    }

    while(1){
        newBorrow.borrowReturn.day = getIntInput("Nhap ngay tra sach: ",1,31);
        newBorrow.borrowReturn.month = getIntInput("Nhap thang tra sach: ",1,12);
        newBorrow.borrowReturn.year = getIntInput("Nhap nam tra sach: ",1900,9999);

        int maxDayReturn = getDaysInMonth(newBorrow.borrowReturn.month,newBorrow.borrowReturn.year);
        if(newBorrow.borrowReturn.day <= maxDayReturn){
            break;
        }
        printf("Ngay tra khong hop le. Vui long nhap lai\n");
    }
    
    getStringInput("Nhap ten nguoi muon: ",newBorrow.borrowerName, sizeof(newBorrow.borrowerName));
    
    while(1){
        newBorrow.status = getIntInput("Nhap trang thai (0 = da tra, 1 = dang muon): ",0,1);
        if(newBorrow.status == 0 || newBorrow.status == 1){
            break;
        }
        printf("Trang thai khong hop le.Chi duoc 0 hoac 1\n");
    }
    if (newBorrow.status == 1){
        int index = findBookIndexById(newBorrow.bookId);
        books[index].quantity--;
    }
    borrows[borrowCount++] = newBorrow;
    printf("Them moi phieu muon thanh cong (ID: %d)\n",newBorrow.borrowId);
}
void returnBook(){
	if(borrowCount == 0){
		printf("Danh sach phieu muon trong\n");
		return;
	}
	int id = getIntInput("Nhap Borrow ID can tra sach: ",2000,2000 + MAX_BORROWS -1);
	int index =findBorrowIndexById (id);
	
	if (index == -1){
		printf("Khong tim thay phieu muon\n");
		return;
	}
	struct Borrow *br = &borrows[index];
	if(br->status == 0){
		printf("Phieu muon nay da duoc tra truoc do\n");
		return;
	}
	printf("Nhap ngay tra moi:\n");
	int day,month,year;
	while(1){
		day = getIntInput("Ngay: ",1,31);
		month = getIntInput("Thang: ",1,12);
		year = getIntInput("Nam: ",1900,9999);
		
		int maxDay = getDaysInMonth(month, year);
		if(day > maxDay){
			printf("Ngay khong hop le.Vui long nhap lai\n");
			continue;
		}
		if(year < br->borrowDate.year || 
		  (year == br->borrowDate.year && month < br->borrowDate.month) || 
		  (year == br->borrowDate.year && month == br->borrowDate.month && day < br->borrowDate.day)){
		  	printf("Thoi gian tra phai sau ngay muon\n");
		  	continue;
		  }
		  break;
	}
	br->borrowReturn.day = day;
	br->borrowReturn.month = month;
	br->borrowReturn.year = year;
	br->status = 0;
	
	int bookIndex = findBookIndexById(br->bookId);
	if(bookIndex != -1){
		books[bookIndex].quantity++;
	}
	printf("Tra sach thanh cong\n");
}
void displayBorrowSlips(){
	if(borrowCount == 0){
		printf("Danh sach phieu muon trong\n");
		return;
	}
	const int ITEMS =10;
	int page = 1;
	int totalPage = (borrowCount % ITEMS ==0) ? (borrowCount / ITEMS) : (borrowCount / ITEMS + 1);
	while(1){
		int start = (page -1) *ITEMS;
		int end = start + ITEMS;
		printf("\n========= Trang %d / %d =========\n", page, totalPage);
        printf("+--------+--------+------------+------------+------------------------------+-----------+\n");
        printf("|%-8s|%-8s|%-12s|%-12s|%-30s|%-11s|\n", "BrwID", "BookID", "Ngay muon", "Ngay tra", "Nguoi muon", "Status");
        printf("+--------+--------+------------+------------+------------------------------+-----------+\n");
        
        for(int i = start; i<end && i< borrowCount; i++){
        	struct Borrow *br =&borrows[i];
        	
        	char statusText[20];
        	if(br->status == 0){
        		strcpy(statusText,"da tra");
			}else{
				strcpy(statusText,"dang muon");
			}
			printf("|%-8d|%-8d|%02d/%02d/%04d  |%02d/%02d/%04d  |%-30s|%-11s|\n",
			br->borrowId,br->bookId,br->borrowDate.day,br->borrowDate.month,br->borrowDate.year,
			br->borrowReturn.day,br->borrowReturn.month,br->borrowReturn.year,
			br->borrowerName,statusText 
			);
		}
		 printf("+--------+--------+------------+------------+------------------------------+-----------+\n");
		 printf("1.Trang truoc\n");
		 printf("2.Quya lai menu\n");
		 printf("3.Trang sau\n");
		 
		 int choice = getIntInput("Nhap lua chon (1-3): ",1,3);
		 if(choice == 2){
		 	return; 
		 } 
		 if(choice == 1 && page > 1){
		 	page--; 
		 } 
		 if(choice==3 && page <totalPage){
		 	page++; 
		 } 
	}
}

void initSampleData(){ // them sach
	struct Book b1 = {nextBookId++, "Tho","Thanh An",2022,55};
	struct Book b2 = {nextBookId++,"Cao","Thu cuc",2005,25};
	struct Book b3 = {nextBookId++,"Rai ca","abc",2003,15};
	
	books[bookCount++] = b1;
	books[bookCount++] = b2;
	books[bookCount++] = b3;
	
	struct Borrow br1 ={nextBorrowId++,b1.bookId,{05,11,2024},{0,0,0},"Cuc xinh dep",1};
	struct Borrow br2 ={nextBorrowId++,b2.bookId,{05,12,2021},{5,1,2022},"Cuc rai ca",0};
	
	borrows[borrowCount++] = br1;
	borrows[borrowCount++] = br2;
	//giam so luong sach neu dg co ng muon
	if(br1.status == 1){
		int index = findBookIndexById(br1.bookId);
		books[index].quantity--;
	}
	};


int main(){
	initSampleData(); // them du lieu mau 
    int choice;
    do{
        printf("\n============ MENU =================\n");
        printf("|1. Them moi sach                  |\n");
        printf("|2. Cap nhat thong tin sach        |\n");
        printf("|3. Hien thi danh sach             |\n");
        printf("|4. Xoa thong tin sach             |\n");
        printf("|5. Tim kiem sach                  |\n");
        printf("|6. Them moi phieu muon            |\n");
        printf("|7. Tra sach                       |\n");
        printf("|8. Hien thi danh sach phieu muon  |\n");
        printf("|0. Thoat                          |\n");
        choice = getIntInput("Nhap lua chon: ", 0,9);
        switch(choice){
            case 1: addBook(); break;
            case 2: updateBook(); break;
            case 3: displayBooks(); break;
            case 4: deleteBook(); break;
            case 5: searchBook(); break;
            case 6: addBorrowSlip(); break;
            case 7: returnBook(); break;
            case 8: displayBorrowSlips(); break;
            case 0: printf("Thoat...\n"); break;
            default: printf("Lua chon khong hop le!\n"); break;
        }

    }while(choice != 0);
    return 0;
}


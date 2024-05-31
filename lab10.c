#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ORDERS 20

// функции и структуры из работы 7
struct Date {
    int d, m, y;
};

struct order {
    char factory_name[20];
    struct Date order_receipt;
    int *amount_of_days;
    double *price;
    int payment;
    int condition;
};

void print_date(const struct Date *p) {
    printf("%d.%d.%d", p->d, p->m, p->y);
}

void print(const struct order *p) {
    printf("Factory name:\t%s\n", p->factory_name);
    printf("Date:\t");
    print_date(&p->order_receipt);
    printf("\n");
    printf("Duration of work:\t%d\n", *(p->amount_of_days));
    printf("Price:\t%lg\n", *(p->price));
    printf("Paid:\t%s\n", (p->payment ? "yes" : "no"));
    printf("Done:\t%s\n", (p->condition ? "yes" : "no"));
}

void input_date(struct Date *p) {
    int valid = 0;
    do {
        printf("\tday(xx):\t");
        if (scanf("%d", &p->d) != 1 || p->d < 1 || p->d > 31) {
            printf("Invalid day. Please enter a valid day (1-31).\n");
            while (getchar() != '\n');
        } else {
            valid = 1;
        }
    } while (!valid);

    valid = 0;
    do {
        printf("\tmonth(xx):\t");
        if (scanf("%d", &p->m) != 1 || p->m < 1 || p->m > 12) {
            printf("Invalid month. Please enter a valid month (1-12).\n");
            while (getchar() != '\n');
        } else {
            valid = 1;
        }
    } while (!valid);

    valid = 0;
    do {
        printf("\tyear(xxxx):\t");
        if (scanf("%d", &p->y) != 1 || p->y < 1970 || p->y > 2024) {
            printf("Invalid year. Please enter a valid year (1970-2024).\n");
            while (getchar() != '\n');
        } else {
            valid = 1;
        }
    } while (!valid);
}

void input(struct order *p) {
    printf("Factory name:\t");
    scanf("%20s", p->factory_name);
    printf("Date:\n");
    input_date(&p->order_receipt);
    printf("Duration of work (days):\t");
    p->amount_of_days = malloc(sizeof(int));
    while (scanf("%d", p->amount_of_days) != 1 || *(p->amount_of_days) < 1) {
        printf("Invalid input. Please enter a valid duration.\n");
        while (getchar() != '\n');
    }
    printf("Price:\t");
    p->price = malloc(sizeof(double));
    while (scanf("%lf", p->price) != 1) {
        printf("Invalid input. Please enter a valid price.\n");
        while (getchar() != '\n');
    }
    printf("Paid (0 - no, 1 - yes):\t");
    while (scanf("%d", &p->payment) != 1 || (p->payment != 0 && p->payment != 1)) {
        printf("Invalid input. Please enter 0 or 1.\n");
        while (getchar() != '\n');
    }
    printf("Done (0 - no, 1 - yes):\t");
    while (scanf("%d", &p->condition) != 1 || (p->condition != 0 && p->condition != 1)) {
        printf("Invalid input. Please enter 0 or 1.\n");
        while (getchar() != '\n');
    }
}

double sum_unfulfilled_paid_orders(const struct order *orders, int num_orders) {
    double sum = 0.0;
    int i;
    for (i = 0; i < num_orders; i++) {
        if (orders[i].payment && !orders[i].condition) {
            sum += *(orders[i].price);
        }
    }
    return sum;
}

int total_duration_unpaid_orders(const struct order *orders, int num_orders) {
    int total_duration = 0;
    int i;
    for (i = 0; i < num_orders; i++) {
        if ((!orders[i].payment)) {
            total_duration += *(orders[i].amount_of_days);
        }
    }
    return total_duration;
}


int comp_date(const struct order *p1, const struct order *p2) {
	if (p1->order_receipt.y < p2->order_receipt.y)
		return -1;
	else if (p1->order_receipt.y > p2->order_receipt.y)
		return 1;
	else {
		if (p1->order_receipt.m < p2->order_receipt.m)
			return -1;
	else if (p1->order_receipt.m > p2->order_receipt.m)
		return 1;
	else {
		if (p1->order_receipt.d < p2->order_receipt.d)
			return -1;
	else if (p1->order_receipt.d > p2->order_receipt.d)
		return 1;
	else
		return 0;
	}
	}
}
void sort_by_order_receipt(struct order *orders, int num_orders) {
	qsort(orders, num_orders, sizeof(struct order), (int(*)(const void*, const void*))comp_date);
}

int search_by_company(const struct order *orders, int num_orders, const char *company, int *found) {
    *found = 0;
    int i;
    for (i = 0; i < num_orders; i++) {
        if (strstr(orders[i].factory_name, company) != NULL) {
            printf("\nOrder info:\n");
            print(&orders[i]);
            *found = 1;
        }
    }
    if (*found == 0) {
        printf("No orders found for factory %s\n", company);
    }
    return 1;
}

// функции из работы 8

void print_array(const struct order orders[], int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		printf("\n*** Record %d ***\n", i+1);
		print(&orders[i]);
	}
}
int input_number(int max)
{
	int k;
	printf("Enter number of record (from 1 to %d): ", max);
	while (1)
	{
		fflush(stdin);
		if (scanf("%d", &k) == 1)
			break;
	printf("\aWrong input (not a number). Try again: ");
	}
	if (k <= 0 || k > max)
	{
		printf("\aNumber %d is out of range!\n", k);
		k = 0;
	}
return k;
}
void display_order(struct order orders[], int n, int k)
{
	if (k>=1 && k <= n)
	{
		print(&orders[k-1]);
	}
}


//работа с файлами (работа 9)

int load_from_file(struct order *orders, int max_orders, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Error opening file for reading.\n");
        return 0;
    }

    int num_orders;
    fscanf(f, "%d\n", &num_orders);
    if (num_orders > max_orders) {
        printf("Error: too many orders in file.\n");
        fclose(f);
        return 0;
    }

/*не разобралась как можно сделать это более коротко, если есть более короткий вариант, буду благодарна за подсказку:)*/
    int i;
    for (i = 0; i < num_orders; i++) {
        fscanf(f, "%s\n", orders[i].factory_name);

        fscanf(f, "%d.%d.%d\n", &orders[i].order_receipt.d, &orders[i].order_receipt.m, &orders[i].order_receipt.y);

        orders[i].amount_of_days = malloc(sizeof(int));
        fscanf(f, "%d\n", orders[i].amount_of_days);

        orders[i].price = malloc(sizeof(double));
        fscanf(f, "%lf\n", orders[i].price);

        int payment;
        fscanf(f, "%d\n", &payment);
        orders[i].payment = payment;

        int condition;
        fscanf(f, "%d\n", &condition);
        orders[i].condition = condition;
    }
    fclose(f);
    printf("Data loaded from file %s.\n", filename);
    return num_orders;
}


//тут удаляем все данные из файла (для этого используем режим w, этого достаточно)
void clear_orders(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Error opening file for clearing.\n");
        return;
    }
    fclose(f);
    printf("Data cleared from file %s.\n", filename);
}


int compare_orders(const struct order *o1, const struct order *o2) { //здесь проверяем совпадают ли заказы (понадобится в функции для сохранения заказов в файл)
    if (strcmp(o1->factory_name, o2->factory_name) != 0) {
        return 0;
    }

    if (o1->order_receipt.d != o2->order_receipt.d) {
        return 0;
    }

    if (o1->order_receipt.m != o2->order_receipt.m) {
        return 0;
    }

    if (o1->order_receipt.y != o2->order_receipt.y) {
        return 0;
    }

    if (*(o1->amount_of_days) != *(o2->amount_of_days)) {
        return 0;
    }
    if (fabs(*(o1->price) - *(o2->price)) > 0.01) { //при сравнении значений с плавающей точкой
                                                    //следует использовать функцию fabs и некоторую погрешность,
                                                    //чтобы избежать ошибок, вызванных различиями в представлении чисел с плавающей точкой.
        return 0;
    }

    if (o1->payment != o2->payment) {
        return 0;
    }

    if (o1->condition != o2->condition) {
        return 0;
    }

    return 1;
}


void save_to_file(const struct order *orders, int num_orders, const char *filename) {

    //здесь создаем массив заказов из файла
    int max_file_orders = 20;
    struct order file_orders[max_file_orders];
    int num_file_orders = load_from_file(file_orders, max_file_orders, filename);

    // тут добавляем новые заказы в массив, проверяя на повторение
    int i, j;
    for (i = 0; i < num_orders; i++) {
        int duplicate = 0;

        // тут проверяем на совпадение с заказами из файла
        for (j = 0; j < num_file_orders; j++) {
            if (compare_orders(&orders[i], &file_orders[j]) == 1) {
                duplicate = 1;
                break;
            }
        }

        //проверяем на совпадение с новыми заказами
        for (j = 0; j < i; j++) {
            if (compare_orders(&orders[i], &orders[j]) == 1) {
                duplicate = 1;
                break;
            }
        }

        //если заказ не повторяется, добавляем в массив
        if (!duplicate) {
            if (i < num_file_orders) {
                file_orders[i] = orders[i];
            } else {
                file_orders[num_file_orders++] = orders[i];
            }
        }
    }

    //и только теперь сохраняем в файл
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(f, "%d\n", num_file_orders);

/*не разобралась как можно сделать это более коротко, если есть более короткий вариант, буду благодарна за подсказку:)*/
    for ( i = 0; i < num_file_orders; i++) {
        fprintf(f, "%s\n", file_orders[i].factory_name);

        fprintf(f, "%d.%d.%d\n", file_orders[i].order_receipt.d, file_orders[i].order_receipt.m, file_orders[i].order_receipt.y);

        fprintf(f, "%d\n", *(orders[i].amount_of_days));

        fprintf(f, "%.2f\n", *(orders[i].price));

        fprintf(f, "%d\n", file_orders[i].payment);

        fprintf(f, "%d\n", file_orders[i].condition);
    }

    fclose(f);
    printf("Data saved to file %s.\n", filename);
}


void free_order(struct order *p) {
    free(p->amount_of_days);
    free(p->price);
}

int main(){
	struct order orders[20];
	int n = 0, key, k, i;
	char buf[80];
	char searchName[20];
	do{
        printf("\nChoose operation:\n\t 1 - Add record\n\t 2 - Insert record\n\t"
        " 3 - Delete record\n\t 4 - View record\n\t 5 - View all records\n\t"
        " 6 - Price of all unfinished but paid orders\n\t 7 - Duration of unpaid orders\n\t"
        " 8 - Find by name of factory\n\t 9 - Sorting by date of order\n\t"
        " 10 - Save to file\n\t 11 - Load from file\n\t 12 - Delete all records from file\n\t"
        " 0 - Exit\n> ");

		fflush(stdin);
		if (scanf("%d", &key) != 1)
		key = -1;

		switch (key)
		{
			case 1:
				if (n < 20)
				{
					printf("Input order %d:\n", n+1);
					input(&orders[n]);
					n++;
				}
				else
					printf("\aOut of memory!\n");
			break;

			case 2:
				if (n < 20)
				{
					k = input_number(n) - 1;
					if (k >= 0)
					{
						for (i = n - 1; i >= k; i--) {
							orders[i + 1] = orders[i];
						}
						printf("Input record %d:\n", k+1);
						input(&orders[k]);
						n++;
					}
				}
				else
					printf("\aOut of memory!\n");
			break;

			case 3:
				    k = input_number(n) - 1;
                    if (k<=n){
                        free_order(&orders[k]);
                        for (i = k; i < n - 1; i++)
                        {
                            orders[i] = orders[i + 1];
                        }
                        n--;
                        printf("Order %d has been deleted.\n", k + 1);
                    }
                    else{
                        printf("Wrong number!");
                    }
			break;


			case 4:
				k = input_number(n);
				display_order(orders, n, k);
			break;

			case 5:
				print_array(orders, n);
			break;

			case 6:
				printf("Price of all unfinished but paid orders: %.2lf\n", sum_unfulfilled_paid_orders(orders, n));
			break;


			case 7:
				printf("Duration of unpaid orders: %d days\n", total_duration_unpaid_orders(orders, n));
			break;

            case 8:
                int found = 0;
                printf("Enter factory name to search for orders: ");
                scanf("%20s", searchName);
                search_by_company(orders, n, searchName, &found);
            break;


			case 9:
				sort_by_order_receipt(orders, n);
				printf("\nSorted orders by date:\n");
				print_array(orders, n);
			break;

			case 10:
                printf("Enter file name: ");
                scanf("%s", buf);
                save_to_file(orders, n, buf);
            break;

            case 11:
                printf("Enter file name: ");
                scanf("%s", buf);
                k = load_from_file(orders, MAX_ORDERS, buf);
                if (k > 0) {
                    n = k;
                }
            break;

            case 12:
                printf("Enter file name: ");
                scanf("%s", buf);
                clear_orders(buf);
            break;


			case 0:
			break;
		}
	}
	while (key);
    for (i = 0; i < n; i++) {
        free_order(&orders[i]);
    }
	return 0;
}

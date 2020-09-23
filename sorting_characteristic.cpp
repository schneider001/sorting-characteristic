#include "TXlib.h"
#include <stdlib.h>
#include <ctime>
#include <string>


struct CoordSys {
	int x0, y0;
	double scaleX, scaleY;
};


double* to_pixels(int x, int y, const CoordSys* coord);
void draw_point(int x, int y, const CoordSys* coord);
int bubbleSort(double* num, int size);
int selectionSort(double* num, int size);
double* get_random_numbers(int size, int elem_for_random);
int count_of_comparison(int size);
void start_window();
void create_working_space();
void draw_sort(int key);
void run_program();


int main() {
	start_window();
	run_program();
	
}


double* to_pixels(int x, int y, const CoordSys* coord) {
	double* rec_coord = new double[2];
	rec_coord[0] = x * coord->scaleX + coord->x0;
	rec_coord[1] = coord->y0 - y * coord->scaleY;
	return rec_coord;
}


void draw_point(int x, int y, const CoordSys* coord) {
	double* rec_coord = to_pixels(x, y, coord);
	if (rec_coord[1] >= 55) {
		txCircle(rec_coord[0], rec_coord[1], 3);
	}
	delete[] rec_coord;
}


int bubbleSort(double* num, int size) {
	int count_of_exchange = 0;
	for (int i = 0; i < size - 1; i++) {
		for (int j = (size - 1); j > i; j--) {
			if (num[j - 1] > num[j]) {
				double temp = num[j - 1];
				num[j - 1] = num[j];
				num[j] = temp;
				count_of_exchange += 1;
			}
		}
	}
	return count_of_exchange;
}


int selectionSort(double* num, int size) {
	int count_of_exchange = 0;
	for (int i = 0; i < size - 1; i++) {
		int min = i;
		for (int j = i + 1; j < size; j++) {
			if (num[j] < num[min])
				min = j;
		}
		if (min != i)
			count_of_exchange += 1;
		double temp = num[i];
		num[i] = num[min];
		num[min] = temp;
	}
	return count_of_exchange;
}


double* get_random_numbers(int size, int elem_for_random) {
	srand(elem_for_random);
	double* koef = new double[size];
	for (int i = 0; i < size; i++) {
		koef[i] = 1 + rand() % 1000;
	}
	return koef;
}


int count_of_comparison(int size) {
	return (size * size - size) / 2;
}


void start_window() {
	txCreateWindow(1200, 700);
	create_working_space();
	txDrawText(475, 625, 750, 700, "click to start sorting");
}


void create_working_space() {
	txSetFillColor(TX_WHITE);
	txSetColor(TX_LIGHTGREEN);
	txRectangle(40, 50, 550, 560);
	txRectangle(640, 50, 1150, 560);
	txSetFillColor(RGB(0, 191, 255));
	txRectangle(575, 575, 625, 625);

	for (int line = 60; line < 550; line += 10) {
		txLine(line, 50, line, 550);
		txLine(line + 600, 50, line + 600, 550);
	}

	for (int line = 540; line > 50; line -= 10) {
		txLine(50, line, 550, line);
		txLine(650, line, 1150, line);
	}

	txSetColor(TX_BLACK);
	txLine(50, 50, 50, 560);
	txLine(40, 550, 550, 550);
	txLine(650, 50, 650, 560);
	txLine(640, 550, 1150, 550);

	txLine(50, 50, 55, 55);
	txLine(50, 50, 45, 55);

	txLine(650, 50, 655, 55);
	txLine(650, 50, 645, 55);

	txLine(545, 545, 550, 550);
	txLine(545, 555, 550, 550);

	txLine(1145, 545, 1150, 550);
	txLine(1145, 555, 1150, 550);

	txSetColor(TX_LIGHTGREEN);
}


void draw_sort(int key) {
	srand(time(0));
	int elem_for_random = 1 + rand() % 1000000;

	int count_of_exchange = 0;
	int count_of_comp = 0;
	double scaleY_for_exchange = 0.25;

	for (int size_of_array = 10; size_of_array <= 2000; size_of_array += 10) {
		elem_for_random += size_of_array;

		double* sortable_array = get_random_numbers(size_of_array, elem_for_random);

		if (key == 0) {
			count_of_exchange = selectionSort(sortable_array, size_of_array);
			txDrawText(500, 15, 700, 25, "Selection Sort:");
		}
		else {
			count_of_exchange = bubbleSort(sortable_array, size_of_array);
			scaleY_for_exchange = 0.001;
			txDrawText(500, 15, 700, 25, "Bubble Sort:");
		}

		count_of_comp = count_of_comparison(size_of_array);

		const CoordSys left_graph = { 50, 550, 0.35, 0.001 };
		const CoordSys right_graph = { 650, 550, 0.25, scaleY_for_exchange };

		draw_point(size_of_array, count_of_comp, &left_graph);
		draw_point(size_of_array, count_of_exchange, &right_graph);

		txDrawText(10, 575, 550, 600, "dependence the num of comp on the num of elem ");
		txDrawText(690, 575, 1100, 600, "dependence the num of exch on the num of elem ");
		txDrawText(475, 625, 750, 700, "click to change sorting");

		delete[] sortable_array;
	}
}


void run_program() {

	int count_of_push = 0;

	while (txMouseButtons() != 3) {

		int push = txMouseButtons();

		if (txMouseX() >= 575 && txMouseX() <= 625 && txMouseY() >= 575 && txMouseY() <= 625 && push == 1) {
			while (txMouseButtons() == 1) {
				txSleep(1);
			}
			txSetFillColor(TX_BLACK);
			txClear();

			create_working_space();
			count_of_push += 1;
			draw_sort(count_of_push % 2);
		}
		push = 0;
	}
}
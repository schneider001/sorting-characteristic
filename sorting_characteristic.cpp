#include "TXlib.h"
#include <stdlib.h>
#include <ctime>
#include <string>


class CoordSys {
	int x0_, y0_;
	double scaleX_, scaleY_;
public:
	double* to_pixels(int x, int y);
	void draw_point(int x, int y, int top_border_of_area, int right_border_of_area, COLORREF color_of_point);
	CoordSys(int x0, int y0, double scaleX, double scaleY) {
		x0_ = x0;
		y0_ = y0;
		scaleX_ = scaleX;
		scaleY_ = scaleY;
	}
};


class CoordWindow {
	int x0_, y0_, x1_, y1_;
	const char* signature_;
public:
	void draw_window();
	void draw_grid();
	void draw_axis();
	void set_color_back();
	void write_signature();
	int return_top_border();
	int return_right_border();
	CoordWindow(int x0, int y0, int x1, int y1, const char* signature) {
		x0_ = x0;
		y0_ = y0;
		x1_ = x1;
		y1_ = y1;
		signature_ = signature;
	}
};


class Button {
	int x0_, y0_, x1_, y1_;
	const char* name_;
public:
	void draw_button();
	bool if_button_pressed();
	Button(int x0, int y0, int x1, int y1, const char* name) {
		x0_ = x0;
		y0_ = y0;
		x1_ = x1;
		y1_ = y1;
		name_ = name;
	}
};


int bubbleSort(double* num, int size);

int selectionSort(double* num, int size);

double* get_random_numbers(int size, int elem_for_random);

int count_of_comparison(int size);

void create_working_space(	CoordWindow* left_window,
							CoordWindow* right_window,
							Button* bubble_sort,
							Button* clear,
							Button* selection_sort,
							Button* reduce_scaleY_exhnange,
							Button* add_scaleY_exhnange);

void clear_window();

void draw_sort(	int kind_of_sort,
				double scaleY_for_exchange,
				int* quantity_exchange_select,
				int* quantity_exchange_bubble,
				CoordWindow* left_window,
				CoordWindow* right_window);

void redraw_exchange_graph(	double scaleY_for_exchange,
							int* quantity_exchange_select,
							int* quantity_exchange_bubble,
							int* is_select_pressed_now,
							int* is_bubble_pressed_now,
							CoordWindow* right_window);



int main() {

	CoordWindow left_window(40, 50, 550, 560, "dependence the num of comp on the num of elem ");
	CoordWindow right_window(640, 50, 1150, 560, "dependence the num of exch on the num of elem ");

	Button bubble_sort(200, 600, 400, 650, "Bubble Sort");
	Button clear(500, 600, 700, 650, "Clear");
	Button selection_sort(800, 600, 1000, 650, "Selection Sort");
	Button reduce_scaleY_exhnange(1050, 600, 1075, 625, "-");
	Button add_scaleY_exhnange(1075, 600, 1100, 625, "+");

	txCreateWindow(1200, 700);
	create_working_space(	&left_window,
							&right_window,
							&bubble_sort,
							&clear,
							&selection_sort,
							&reduce_scaleY_exhnange,
							&add_scaleY_exhnange);

	int is_select_pressed_now = 0;
	int is_bubble_pressed_now = 0;

	double scaleY_for_exchange = 0.01;

	const int two_butthon_pressed = 3;

	int quantity_exchange_select[200] = { 0 };
	int quantity_exchange_bubble[200] = { 0 };


	while (txMouseButtons() != two_butthon_pressed) {

		const int nosort_flag = 5;
		int kind_of_sort = nosort_flag;

		if (bubble_sort.if_button_pressed()) {
			kind_of_sort = 1;
			is_bubble_pressed_now = 1;
		}
		if (selection_sort.if_button_pressed()) {
			kind_of_sort = 0;
			is_select_pressed_now = 1;
		}
		if (clear.if_button_pressed()) {
			clear_window();
			create_working_space(&left_window,
				&right_window,
				&bubble_sort,
				&clear,
				&selection_sort,
				&reduce_scaleY_exhnange,
				&add_scaleY_exhnange);
			is_select_pressed_now = 0;
			is_bubble_pressed_now = 0;
		}
		if (kind_of_sort != nosort_flag) {
			draw_sort(kind_of_sort,
				scaleY_for_exchange,
				quantity_exchange_select,
				quantity_exchange_bubble,
				&left_window,
				&right_window);
		}

		bool add = add_scaleY_exhnange.if_button_pressed();
		bool reduce = reduce_scaleY_exhnange.if_button_pressed();
		if (add || reduce) {
			if (add) {
				scaleY_for_exchange *= 1.5;
			}
			if (reduce) {
				scaleY_for_exchange /= 1.5;
			}
			redraw_exchange_graph(scaleY_for_exchange,
				quantity_exchange_select,
				quantity_exchange_bubble,
				&is_select_pressed_now,
				&is_bubble_pressed_now,
				&right_window);
		}
	}
}


double* CoordSys::to_pixels(int x, int y) {
	double* rec_coord = new double[2];
	rec_coord[0] = x * scaleX_ + x0_;
	rec_coord[1] = y0_ - y * scaleY_;
	return rec_coord;
}


void CoordSys::draw_point(int x, int y, int top_border_of_area, int right_border_of_area, COLORREF color_of_point) {
	txSetColor(TX_LIGHTGREEN);
	txSetFillColor(color_of_point);
	int point_radius = 3;
	double* rec_coord = to_pixels(x, y);
	if (rec_coord[1] >= top_border_of_area && rec_coord[0] <= right_border_of_area) {
		txCircle(rec_coord[0], rec_coord[1], point_radius);
	}
	delete[] rec_coord;
}


void CoordWindow::draw_window() {
	txSetColor(TX_LIGHTGREEN);

	txRectangle(x0_, y0_, x1_, y1_);
}

void CoordWindow::draw_grid() {
	for (int line = x0_; line < x1_; line += 10) {
		txLine(line, y0_, line, y1_);
	}
	for (int line = y0_; line < y1_; line += 10) {
		txLine(x0_, line, x1_, line);
	}
}


void CoordWindow::draw_axis() {
	txSetColor(TX_BLACK);

	txLine(x0_ + 10, y0_, x0_ + 10, y1_);
	txLine(x0_, y1_ - 10, x1_, y1_ - 10);

	txLine(x0_ + 10, y0_, x0_ + 15, y0_ + 5);
	txLine(x0_ + 10, y0_, x0_ + 5, y0_ + 5);

	txLine(x1_ - 5, y1_ - 15, x1_, y1_ - 10);
	txLine(x1_ - 5, y1_ - 5, x1_, y1_ - 10);
}


void CoordWindow::set_color_back() {
	txSetFillColor(TX_WHITE);
}


void CoordWindow::write_signature() {
	txDrawText(x0_- 30, y1_ + 15, x1_, y1_ + 30, signature_);
}


int CoordWindow::return_top_border() {
	return y0_;
}


int CoordWindow::return_right_border() {
	return x1_;
}


void Button::draw_button() {
	txSetFillColor(TX_BLACK);
	txRectangle(x0_, y0_, x1_, y1_);
	txDrawText(x0_, y0_, x1_, y1_, name_);
}


bool Button::if_button_pressed() {
	if (txMouseX() >= x0_ && txMouseX() <= x1_ && txMouseY() >= y0_ && txMouseY() <= y1_ && txMouseButtons() == 1) {
		while (txMouseButtons() == 1) {
			txSleep(1);
		}
		return TRUE;
	}
	else {
		return FALSE;
	}
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


void create_working_space(	CoordWindow* left_window,
							CoordWindow* right_window,
							Button* bubble_sort,
							Button* clear,
							Button* selection_sort,
							Button* reduce_scaleY_exhnange,
							Button* add_scaleY_exhnange) {
	(*left_window).set_color_back();
	(*right_window).set_color_back();

	(*left_window).draw_window();
	(*right_window).draw_window();

	(*bubble_sort).draw_button();
	(*clear).draw_button();
	(*selection_sort).draw_button();
	(*reduce_scaleY_exhnange).draw_button();
	(*add_scaleY_exhnange).draw_button();

	(*left_window).draw_grid();
	(*right_window).draw_grid();

	(*left_window).draw_axis();
	(*right_window).draw_axis();
}


void clear_window() {
	txSetFillColor(TX_BLACK);
	txClear();
}


void draw_sort(	int kind_of_sort,
				double scaleY_for_exchange,
				int* quantity_exchange_select,
				int* quantity_exchange_bubble,
				CoordWindow* left_window,
				CoordWindow* right_window) {
	srand(time(0));
	int elem_for_random = 1 + rand() % 1000000;

	int count_of_exchange = 0;
	int count_of_comp = 0;
	COLORREF color_of_point = RGB(0, 191, 255);

	for (int size_of_array = 10; size_of_array <= 2000; size_of_array += 10) {

		elem_for_random += size_of_array;

		double* sortable_array = get_random_numbers(size_of_array, elem_for_random);

		if (kind_of_sort == 0) {
			count_of_exchange = selectionSort(sortable_array, size_of_array);
			quantity_exchange_select[size_of_array / 10 - 1] = count_of_exchange;
		}
		else {
			count_of_exchange = bubbleSort(sortable_array, size_of_array);
			quantity_exchange_bubble[size_of_array / 10 - 1] = count_of_exchange;
			color_of_point = TX_GREEN;
		}

		count_of_comp = count_of_comparison(size_of_array);

		CoordSys left_graph ( 50, 550, 0.35, 0.001 );
		CoordSys right_graph ( 650, 550, 0.25, scaleY_for_exchange);


		left_graph.draw_point(	size_of_array,
								count_of_comp,
								(*left_window).return_top_border() + 10,
								(*left_window).return_right_border() - 10,
								color_of_point);
		right_graph.draw_point(	size_of_array,
								count_of_exchange,
								(*right_window).return_top_border() + 10,
								(*right_window).return_right_border() - 10,
								color_of_point);


		(*left_window).write_signature();
		(*right_window).write_signature();

		delete[] sortable_array;
	}
}

void redraw_exchange_graph(	double scaleY_for_exchange,
							int* quantity_exchange_select, 
							int* quantity_exchange_bubble,
							int* is_select_pressed_now,
							int* is_bubble_pressed_now,
							CoordWindow* right_window) {

	CoordSys right_graph(650, 550, 0.25, scaleY_for_exchange);

	(*right_window).set_color_back();
	(*right_window).draw_window();
	(*right_window).draw_grid();
	(*right_window).draw_axis();

	for (int size_of_array = 10; size_of_array <= 2000; size_of_array += 10) {

		if (*is_select_pressed_now == 1) {
			right_graph.draw_point(	size_of_array,
									quantity_exchange_select[size_of_array / 10 - 1],
									(*right_window).return_top_border() + 10, 
									(*right_window).return_right_border() - 10,
									RGB(0, 191, 255));
		}
		if (*is_bubble_pressed_now == 1) {
			right_graph.draw_point(	size_of_array,
									quantity_exchange_bubble[size_of_array / 10 - 1],
									(*right_window).return_top_border() + 10,
									(*right_window).return_right_border() - 10,
									TX_GREEN);
		}
	}
}



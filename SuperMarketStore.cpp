#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <stdlib.h>

using namespace std;

string STORE_NAME = "Satram Novelity Store";
tm *getTime()
{
	time_t now = time(0);
	return localtime(&now);
}

class product
{
	int product_num;
	char product_name[50];
	int quantity;
	float sell_p;
	float buy_p;
	int gst;
	float total;
	float profit;

  public:
	void calculate()
	{
		this->total = this->quantity * (this->sell_p);
		this->profit = this->total - (this->buy_p * this->quantity) * (1 + this->gst / 100);
	}
	product()
	{
		product_num = 0;
		product_name[0] = '\0';
	}
	product(int p_num, char *name, int quantity, float sell_p, float buy_p, int gst)
	{
		this->product_num = p_num;
		strcpy(this->product_name, name);
		this->quantity = quantity;
		this->sell_p = sell_p;
		this->buy_p = buy_p;
		this->gst = gst;
		calculate();
	}
	void get_data()
	{
		float temp_sell_p;
		cout << "Enter Serial Number: ";
		cin >> this->product_num;
		cout << "Enter Product Name(MAX LENGTH: 20): ";
		cin.ignore();
		cin.get(this->product_name, 20);
		cout << "Enter Quantity: ";
		cin >> this->quantity;
		cout << "Enter Buying Price: ";
		cin >> this->buy_p;
		cout << "Enter GST: ";
		cin >> this->gst;
		temp_sell_p = buy_p * ((gst * .01) + 1);
		cout << "Enter Selling Price (Selling Price should be greater than " << temp_sell_p << " to turn profit):";
		cin >> this->sell_p;
		calculate();
	}
	void show_product()
	{
		cout << "Serial: " << this->product_num
			 << "\nName: " << this->product_name
			 << "\nQuantity: " << this->quantity
			 << "\nSelling Price: " << this->sell_p
			 << "\nBuying Price: " << this->buy_p
			 << "\nGST: " << this->gst << endl;
	}
	bool remove_product(int x)
	{
		if (x > this->quantity)
			return false;
		this->quantity = this->quantity - x;
		return true;
	}
	int getSerialNumber()
	{
		return this->product_num;
	}
	char *getName()
	{
		return this->product_name;
	}
	int getQuantity()
	{
		return this->quantity;
	}
	int getGST()
	{
		return this->gst;
	}
	float getTotal()
	{
		return this->total;
	}
	float getProfit()
	{
		return this->profit;
	}
	float getSellPrice()
	{
		return this->sell_p;
	}
	float getBuyPrice()
	{
		return this->buy_p;
	}
	bool isValid()
	{
		if (product_num == 0 || product_name[0] == '\0')
			return false;
		return true;
	}
	friend product getProductBySerial(int);
};

class Item
{
  public:
	int serial;
	char product_name[50];
	int quantity;
	float total;
	tm t;
	Item() {}
	Item(int serial, char *product_name, int quantity, float total)
	{
		this->serial = serial;
		strcpy(this->product_name, product_name);
		this->quantity = quantity;
		this->total = total;
		this->t = *getTime();
	}
	void show_product()
	{
		cout << "\nItem Detail\n";
		cout << "serial: " << serial << "\n"
			 << "Product Name: " << product_name << "\n"
			 << "Quantity: " << quantity << "\n"
			 << "Total: " << total << "\n"
			 << "Time Stamp: " << asctime(&t) << "\n";
	}
};

class ItemStack
{
  public:
	int top;
	int capacity;
	Item *arr;
	ItemStack()
	{
		top = -1;
		capacity = 50;
		arr = (Item *)malloc(sizeof(Item) * capacity);
	}
	void push(Item i)
	{
		arr[++top] = i;
	}
	Item pop()
	{
		return arr[top--];
	}
	void showAll()
	{
		cout << "\n\n";
		for (int i = 0; i <= top; i++)
		{
			arr[i].show_product();
			cout << "\n";
		}
	}
};

product getProductBySerial(int serial)
{
	ifstream fin;
	fin.open("product.dat", ios::in | ios::binary);
	if (!fin)
	{
		cout << "Failed to open file!!";
		exit(0);
	}
	else
	{
		product p;
		fin.read((char *)&p, sizeof(p));
		while (!fin.eof())
		{
			if (serial == p.product_num)
			{
				fin.close();
				return p;
			}
			fin.read((char *)&p, sizeof(p));
		}
		fin.close();
		return product();
	}
}

void showProduct(int serialNumber)
{
	product p = getProductBySerial(serialNumber);
	if (p.isValid())
		p.show_product();
	else
		cout << "\nProduct Doesn't Exist!!!";
}

void AddProduct()
{
	ofstream file;
	file.open("product.dat", ios::out | ios::app | ios::binary);
	product p;
	p.get_data();
	if (p.isValid())
	{
		file.write((char *)&p, sizeof(p));
	}
	file.close();
}

void EditProduct(int serial_num)
{
	bool found = false;
	fstream file;
	product p;
	file.open("product.dat", ios::in | ios::out | ios::binary);
	if (!file)
	{
		cout << "File Not Found Exit!!\nExiting....";
		exit(0);
	}
	else
	{
		file.read((char *)&p, sizeof(p));
		while (!file.eof())
		{
			if (p.getSerialNumber() == serial_num)
			{
				found = true;
				break;
			}
			else
			{
				file.read((char *)&p, sizeof(p));
			}
		}
		if (found)
		{
			file.clear();
			file.seekp(-(sizeof(product)), ios::cur);
			p.show_product();
			cout << "Enter the Values: " << endl;
			cout << "Note: IF DONT WISH TO CHANGE KINDLY ENTER SAME VALUE."<<endl;
			p.get_data();
			file.write((char *)&p, sizeof(p));
		}
		else
		{
			cout << "Product Doesn't Exist!!!!";
		}
	}
	file.close();
}

void confirm_sell(int serial_num, int quantity)
{
	bool found = false;
	fstream file;
	product p;
	file.open("product.dat", ios::in | ios::out | ios::binary);
	if (!file)
	{
		cout << "File Not Found Exit!!\nExiting....";
		exit(0);
	}
	else
	{
		file.read((char *)&p, sizeof(p));
		while (!file.eof())
		{
			if (p.getSerialNumber() == serial_num)
			{
				found = true;
				break;
			}
			else
			{
				file.read((char *)&p, sizeof(p));
			}
		}
		if (found)
		{
			file.clear();
			file.seekp(-(sizeof(product)), ios::cur);
			if (p.remove_product(quantity))
				file.write((char *)&p, sizeof(p));
			else
				cout << "Insufficient Quantity!!\n";
		}
		else
		{
			cout << "Product Doesn't Exist!!!!";
		}
	}
	file.close();
}

void RemoveProduct(int serial_num)
{
	bool found = false;
	ifstream fin;
	ofstream fout;
	fin.open("product.dat", ios::in | ios::binary);
	if (!fin)
	{
		cout << "File Not Found Exit!!\nExiting....";
		exit(0);
	}
	else
	{
		product p;
		fin.read((char *)&p, sizeof(p));
		while (!fin.eof())
		{
			if (p.getSerialNumber() == serial_num)
			{
				found = true;
				break;
			}
			fin.read((char *)&p, sizeof(p));
		}
		if (found)
		{
			fin.clear();
			fin.seekg(0);
			fout.open("temp.dat", ios::binary | ios::out);
			fin.read((char *)&p, sizeof(p));
			while (!fin.eof())
			{
				if (!(p.getSerialNumber() == serial_num))
				{
					fout.write((char *)&p, sizeof(p));
				}
				fin.read((char *)&p, sizeof(p));
			}
			fin.close();
			fout.close();
			remove("product.dat");
			rename("temp.dat", "product.dat");
		}
		else
		{
			cout << "Product Doesn't Exists!!!";
		}
	}
}

void makeSale(Item i)
{
	confirm_sell(i.serial, i.quantity);
	ofstream fout;
	fout.open("saleLedger.dat", ios::binary | ios::app);
	if (fout)
	{
		fout.write((char *)&i, sizeof(i));
		fout.close();
	}
	else
	{
		cout << "Unable To open file!!\n";
		exit(0);
	}
}

void ShowLedger()
{
	ifstream fin;
	fin.open("saleLedger.dat", ios::binary);
	Item i;
	fin.clear();
	fin.read((char *)&i, sizeof(i));
	if (!fin)
	{
		cout << "Unable to open file!!!\n";
		exit(0);
	}
	cout<<"\n******LEDGER******\n";
	while (!fin.eof())
	{
		cout << "\n\nSale On " << i.t.tm_hour
			 << ":" << i.t.tm_min
			 << ":" << i.t.tm_sec
			 << "  " << i.t.tm_mday
			 << "-" << i.t.tm_mon
			 << "-" << i.t.tm_year << "\n";
		cout << "Item No: " << i.serial << "\n";
		cout << "Item Name: " << i.product_name << "\n";
		cout << "Quantity: " << i.quantity << "\n";
		cout << "Total: " << i.total << "\n";
		fin.read((char *)&i, sizeof(i));
	}
	cout<<"\n******END******\n";
	fin.close();
}

void showAllProducts()
{
	ifstream fin;
	fin.open("product.dat", ios::binary | ios::in);
	if (!fin)
	{
		cout << "file not opened\n"
			 << endl;
		exit(0);
	}
	else
	{
		product p;
		cout << "Product Inventory" << endl;
		cout << "S.No\tName\t\t\tQuantity\tGST\tTotal\t\t   Profit" << endl;
		fin.read((char *)&p, sizeof(product));
		while (!fin.eof())
		{
			cout << left << setw(8) << p.getSerialNumber()
				 << setw(24) << p.getName() << setw(16) << p.getQuantity()
				 << setw(8) << p.getGST() << setw(19) << fixed << setprecision(3) << p.getTotal()
				 << p.getProfit() << endl;
			fin.read((char *)&p, sizeof(product));
		}
		fin.close();
	}
}

void ShowAllProductCounter(){
	ifstream fin;
	fin.open("product.dat", ios::binary | ios::in);
	if (!fin)
	{
		cout << "file not opened\n"
			 << endl;
		exit(0);
	}
	else
	{
		product p;
		cout << "\nPRODUCT AVAILABLE\n" << endl;
		cout << "Sr.No\tProduct Name\t\tQuantity\tPrice" << endl;
		fin.read((char *)&p, sizeof(product));
		while (!fin.eof())
		{
			cout << left << setw(8) << p.getSerialNumber() << setw(24) << p.getName() << setw(16) << p.getQuantity() <<fixed<<setprecision(3)<< p.getSellPrice() << endl;
			fin.read((char *)&p, sizeof(product));
		}
		fin.close();
	}
}

int AdminMenu()
{
	int ch;
	cout << "\n\nAdministrator Menu\n1.Show All Products\n2.Add Product\n3.Edit Product\n4.Delete Product\n5.Product Detail\n6.Show Ledger\n7. Return To Main Menu\nEnter Your Choice: ";
	cin >> ch;
	system("clear");
	return ch;
}

void customerMenu()
{
	Item I;
	ItemStack IStack;
	product p;
	int serial, q, grand_total=0;
	float total;
	char c = 'y';
	system("clear");
	ShowAllProductCounter();
	while (c == 'y' || c == 'Y')
	{
		cout << "Enter Serial Number: ";
		cin >> serial;
		cout << "Enter Quantity: ";
		cin >> q;
		p = getProductBySerial(serial);
		if (!p.isValid())
		{
			cout << "Product Doesn't Exist!!";
		}
		else
		{
			cout << "\n"
				 << p.getName() << " added to cart.\n";
			total = p.getSellPrice() * q;
			I = Item(p.getSerialNumber(), p.getName(), q, total);
			IStack.push(I);
		}
		cout << "Want Add More Products(y/n): ";
		cin >> c;
	}
	cout << "\n\n*****INVOICE*****\n";
	cout << "Sr.No\tProduct Name\t\tQuantity\tTotal" << endl;

	while (IStack.top != -1)
	{
		I = IStack.pop();
		cout << left << setw(8) << I.serial << setw(24) << I.product_name << setw(16) << I.quantity << I.total << endl;
		makeSale(I);
		grand_total = grand_total + I.total;
	}
	cout<<"GRAND TOTAL: "<<grand_total<<endl;
	cout<<"***********************************\n\n";
}

int menu()
{
	int ch;
	cout << STORE_NAME << endl;
	cout << "1.Admin\n2.Customer\n3.Exit\n";
	cout << "Enter Your Choice: ";
	cin >> ch;
	system("clear");
	return ch;
}

int main()
{
	int serial;
main_menu:
	while (1)
	{
		switch (menu())
		{
		case 1:
			while (1)
			{
				switch (AdminMenu())
				{
				case 1:
					showAllProducts();
					break;
				case 2:
					AddProduct();
					break;
				case 3:
					cout << "Enter Serial Number To Edit: ";
					cin >> serial;
					EditProduct(serial);
					break;
				case 4:
					cout << "Enter Serial Number To Delete: ";
					cin >> serial;
					RemoveProduct(serial);
					break;
				case 5:
					cout << "Enter Serial Number: ";
					cin >> serial;
					showProduct(serial);
					break;
				case 6:
					ShowLedger();
					break;
				case 7:
					goto main_menu;
					break;
				default:
					cout << "Wrong Choice!!\n";
					break;
				}
			}
			break;
		case 2:
			customerMenu();
			break;
		case 3:
			exit(0);
			break;
		default:
			cout << "Wrong Choice!!";
		}
	}
	return 0;
}

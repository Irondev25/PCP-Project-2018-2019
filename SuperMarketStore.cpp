#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <ctime>

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
	string product_name;
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
		product_name = "\0";
	}
	product(int p_num, string name, int quantity, float sell_p, float buy_p, int gst)
	{
		this->product_num = p_num;
		this->product_name = name;
		this->quantity = quantity;
		this->sell_p = sell_p;
		this->buy_p = buy_p;
		this->gst = gst;
		calculate();
	}
	void get_data()
	{
		cout << "Enter Serial Number: ";
		cin >> this->product_num;
		cout << "Enter Product Name(MAX LENGTH: 20): ";
		cin>>this->product_name;
		cout << "Enter Quantity: ";
		cin >> this->quantity;
		cout << "Enter Buying Price: ";
		cin >> this->buy_p;
		cout << "Enter GST: ";
		cin >> this->gst;
		cout << "Enter Selling Price: (Selling Price should be greater than " << (this->buy_p + (this->gst / 100) * this->buy_p) << " to turn profit)";
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
		this->quantity -= x;
		return true;
	}
	int getSerialNumber()
	{
		return this->product_num;
	}
	string getName()
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
		if (product_num == 0 || product_name == "\0")
			return false;
		return true;
	}
	void save()
	{
		ofstream fout;
		fout.open("product.dat", ios::binary | ios::app);
		if (!fout)
		{
			cout << "Failed To Open File!!\nExiting..";
			exit(0);
		}
		else
		{
			fout.write((char *)this, sizeof(product));
		}
		fout.close();
	}
	void update()
	{
		int s_num = this->product_num;
		fstream file;
		file.open("product.dat", ios::binary | ios::ate | ios::in | ios::out);
		if (!file)
		{
			cout << "Failed to open file!!!\nExiting...." << endl;
			exit(0);
		}
		else
		{
			file.seekg(0);
			file.read((char *)this, sizeof(product));
			while (!file.eof())
			{
				if (s_num == this->product_num)
				{
					cout << "Product Detail: ";
					this->show_product();
					cout << "Enter the Values: " << endl;
					cout << "Note: IF DONT WISH TO CHANGE KINDLY ENTER SAME VALUE.";
					this->get_data();
					file.seekp(-(sizeof(product)),ios::cur);
					file.write((char *)this, sizeof(product));
					break;
				}
				file.read((char *)this, sizeof(product));
			}
		}
		file.close();
	}
	void remove_it()
	{
		int s_num = this->product_num;
		ofstream fout;
		ifstream fin;
		fin.open("product.dat", ios::in | ios::binary);
		if (!fin)
		{
			cout << "Failed to open file!!\nExiting...";
			exit(0);
		}
		else
		{
			fout.open("temp.dat", ios::binary | ios::out);
			fin.read((char *)this, sizeof(product));
			while (!fin.eof())
			{
				if (!(s_num == this->product_num))
				{
					fout.write((char *)this, sizeof(product));
				}
				fin.read((char *)this, sizeof(product));
			}
			fin.close();
			fout.close();
			if(remove("product.dat") == 0)
				cout<<"Sucessfully Deleted file\n";
			else
				cout<<"File Removal Failed\n";
			if(rename("temp.dat", "product.dat") == 0)
				cout<<"Successfully Renamed\n";
			else
				cout<<"Failed to Rename\n";
		}
	}
};

class Item
{
  public:
	int serial;
	string product_name;
	int quantity;
	float total;
	Item(){}
	Item(int serial, string product_name, int quantity, float total)
	{
		this->serial = serial;
		this->product_name = product_name;
		this->quantity = quantity;
		this->total = total;
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
};

product getProductBySerial(int serial)
{
	ifstream fin;
	fin.open("product.dat", ios::in);
	product p;
	fin.read((char *)&p, sizeof(p));
	while (!fin.eof())
	{
		if (serial == p.getSerialNumber())
		{
			return p;
		}
	}
	return product();
}

void AddProduct()
{
	product p;
	p.get_data();
	if (p.isValid())
		p.save();
}

void EditProduct(int serial_num)
{
	bool found = false;
	fstream fin;
	product p;
	fin.open("product.dat", ios::in | ios::out | ios::binary);
	if (!fin)
	{
		cout << "File Not Found Exit!!\nExiting....";
		exit(0);
	}
	else
	{
		fin.read((char *)&p, sizeof(p));
		while (!fin.eof())
		{
			if (p.getSerialNumber() == serial_num)
			{
				found = true;
				break;
			}
			else
			{
				fin.read((char *)&p, sizeof(p));
			}
		}
		if (found)
		{
			p.update();
		}
		else
		{
			cout << "Product Doesn't Exist!!!!";
		}
	}
	fin.close();
}

void RemoveProduct(int serial_num)
{
	bool found = false;
	ifstream fin;
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
			p.remove_it();
		}
		else
		{
			cout << "Product Doesn't Exists!!!";
		}
	}
	fin.close();
}

void makeSale(Item i)
{
	ofstream fout;
	fout.open("saleLedger.dat", ios::binary | ios::app);
	if (fout)
	{
		tm *t = getTime();
		fout.write((char *)t, sizeof(*t));
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
	tm *t = nullptr;
	Item *i = nullptr;
	fin.read((char *)&t, sizeof(*t));
	fin.read((char *)&i, sizeof(*i));
	if (!fin)
	{
		cout << "Unable to open file!!!\n";
		exit(0);
	}
	while (!fin.eof())
	{
		cout << "\n\nSale On " << t->tm_hour
			 << ":" << t->tm_min
			 << ":" << t->tm_sec
			 << "  " << t->tm_mday
			 << "-" << t->tm_mon
			 << "-" << t->tm_year << "\n";
		cout << "Item No: " << i->serial << "\n";
		cout << "Item Name: " << i->product_name << "\n";
		cout << "Quantity: " << i->quantity << "\n";
		cout << "Total: " << i->total << "\n";
		fin.read((char *)&t, sizeof(*t));
		fin.read((char *)&i, sizeof(*i));
	}
	fin.close();
}

void showAllProducts()
{
	ifstream fin;
	fin.open("product.dat", ios::binary|ios::in);
	product p;
	cout << "Product Inventory" << endl;
	cout << "S.No\tName\t\t\tQuantity\tGST\tTotal\t\t   Profit" << endl;
	fin.seekg(0);
	fin.read((char *)&p, sizeof(p));
	while (!fin.eof())
	{
		cout << left << setw(8) << p.getSerialNumber()
			 << setw(24) << p.getName() << setw(16) << p.getQuantity()
			 << setw(8) << p.getGST() << setw(19) << fixed << setprecision(3) << p.getTotal()
			 << p.getProfit() << endl;
		fin.read((char *)&p, sizeof(p));
	}
	fin.close();
	cout<<"\nfile closed done\n";
}

int AdminMenu()
{
	int ch;
	cout << "\n\nAdministrator Menu\n1.Show All Products\n2.Add Product\n3.Edit Product\n4.Delete Product\n5. Return To Main Menu\nEnter Your Choice: ";
	cin >> ch;
	return ch;
}

void customerMenu()
{
	Item *I, i;
	ItemStack IStack;
	int serial, q;
	float total;
	char c = 'y';
	showAllProducts();
	while (c == 'y' || c == 'Y')
	{
		cout << "Enter Serial Number: ";
		cin >> serial;
		cout << "Enter Quantity: ";
		cin >> q;
		product p = getProductBySerial(serial);
		if (!p.isValid())
		{
			cout << "Product Doesn't Exist!!";
		}
		else
		{
			total = p.getSellPrice() * q;
			I = new Item(serial, p.getName(), q, total);
			IStack.push(*I);
		}
		cout << "Want Add More Products: ";
		cin >> c;
	}
	cout << "Sr.No\tProduct Name\t\tQuantity\tTotal" << endl;

	while (IStack.top != -1)
	{
		i = IStack.pop();
		cout << left << setw(8) << I->serial << setw(24) << I->product_name << setw(16) << I->quantity << I->total;
		makeSale(i);
	}
}

int menu()
{
	int ch;
	cout << STORE_NAME << endl;
	cout << "1.Admin\n2.Customer\n3.Exit\n";
	cout << "Enter Your Choice: ";
	cin >> ch;
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
			while(1){
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
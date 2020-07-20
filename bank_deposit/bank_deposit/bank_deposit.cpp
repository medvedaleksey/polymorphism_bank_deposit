#include <iostream>

using namespace std;

class BDeposit
{
protected:
	int money;
	int term;
	int rate;
public:
	BDeposit()
	{
		term = 0;
		rate = 0;
		money = 0;
	}
	BDeposit(int t, int r, int m)
	{
		if (t <= 0 || r <= 0 || m <= 0) { abort(); }
		term = t;
		rate = r;
		money = m;
	}
	BDeposit(BDeposit& dep)
	{
		term = dep.term;
		rate = dep.rate;
		money = dep.money;
	}
	virtual double Calculation() = 0;
};
class Standard_Monthly : public BDeposit
{
private:
	const double tax = 0.195;
	int monthly;
public:
	Standard_Monthly() :BDeposit()
	{
		monthly = 0;
	}
	Standard_Monthly(int t, int r, int m, int mon) : BDeposit(t, r, m)
	{
		if (mon > 0) {
			monthly = mon;
		}
		else abort();
	}
	Standard_Monthly(Standard_Monthly& st) :BDeposit(st.term, st.rate, st.money)
	{
		monthly = st.monthly;
	}
	virtual double Calculation() {
		double sum = 0, new_money = 0;
		for (int i = 0; i < term; i++)
		{
			new_money += monthly;
			sum = sum + (new_money * double(rate) / 1200) * (1 - tax);
		}
		sum = sum + new_money;
		sum = round(sum * 100) / 100;
		return sum;
	}
};
class Standard : public BDeposit
{
private:
	const double tax = 0.195;
public:
	Standard() :BDeposit()
	{
	}
	Standard(int t, int r, int m) :BDeposit(t, r, m)
	{
	}
	Standard(Standard& st) :BDeposit(st.term, st.rate, st.money)
	{
	}
	virtual double Calculation() {
		double ans;
		ans = money + (money * term * double(rate) / 1200) * (1 - tax);
		ans = round(ans * 100) / 100;
		return ans;
	}
};
class Standard_tax_free : public BDeposit
{
private:
	int monthly;
public:
	Standard_tax_free() :BDeposit()
	{
		monthly = 0;
	}
	Standard_tax_free(int t, int r, int m, int mon) :BDeposit(t, r, m)
	{
		if (mon >= 0) {
			monthly = mon;
		}
		else { abort(); }
	}
	Standard_tax_free(Standard_tax_free& st) :BDeposit(st.term, st.rate, st.money)
	{
		monthly = st.monthly;
	}
	virtual double Calculation() {
		double ans;
		ans = money + money * term * double(rate) / 1200;
		ans = round(ans * 100) / 100;
		return ans;
	}
};
struct Element
{
	BDeposit* item;
	Element* next;
};
class Container
{
private:
	Element* first;
public:
	Container()
	{
		first = NULL;
	}
	void Add(BDeposit* p) {
		Element* temp = new Element;
		temp->item = p;
		temp->next = first;
		first = temp;
	}
	double Calculation()
	{
		Element* temp;
		temp = first;
		double sum = 0;
		while (temp != NULL)
		{
			sum += temp->item->Calculation();
			temp = temp->next;
		}
		return sum;
	}
};
int main()
{
	Container cont;
	int term, rate, money, num_dep, type, monthly;
	setlocale(LC_ALL, "Russian");
	cout << "Введите количество депозитов" << endl;
	cin >> num_dep;
	if (num_dep == 0) {
		cout << "Рассчитайте выгоду за год при открытии депозита по самым высоким процентам." << endl << "Введите Ваш вклад" << endl;
		cin >> money;
		Standard exam(12, 14, money), example(exam);
		cout << example.Calculation() - money << endl;
		return 0;
	}
	for (int i = 0; i < num_dep; i++)
	{
		cout << "Выберите вид депозита(0-Стандарт безналоговый, 1-Стандарт без ежемесячных пополнений, 2-Стандарт с ежемесячным пополнением)" << endl;
		cin >> type;
		switch (type)
		{
		case 0:
		{
			cout << "Введите срок(в месяцах), ставку(в процентах), начальный вклад, ежемесячное пополнение" << endl;
			cin >> term >> rate >> money >> monthly;
			Standard_tax_free dep_1(term, rate, money, monthly);
			cont.Add(&dep_1);
			cout << dep_1.Calculation() << endl;
		}
		break;
		case 1:
		{
			cout << "Введите срок(в месяцах), ставку(в процентах), начальный вклад" << endl;
			cin >> term >> rate >> money;
			Standard dep_2(term, rate, money);
			cont.Add(&dep_2);
			cout << dep_2.Calculation() << endl;
		}
		break;
		case 2:
		{
			cout << "Введите срок(в месяцах), ставку(в процентах), начальный вклад, ежемесячное пополнение" << endl;
			cin >> term >> rate >> money >> monthly;
			Standard_Monthly dep_3(term, rate, money, monthly);
			cont.Add(&dep_3);
			cout << dep_3.Calculation() << endl;
		}
		break;
		default:
			cout << "Введено неверное значение, попробуйте еще раз" << endl;
			num_dep++;
			break;
		}
	}
	cout << "Общая сумма " << cont.Calculation();
	return 0;
}


#include "unit_class.hpp"

unit_t::unit_t() 
{
	name = "<empty>";
	mail = "<empty>";
	pass = "<empty>";
	desc = "<empty>";
	link = "<empty>";
}

unit_t::unit_t(string nameT, string mailT, string passT, string descT, string linkT)
	{
		name = nameT;
		mail = mailT;
		pass = passT;
		desc = descT;
		link = linkT;
	}

unit_t::unit_t(unit_t const& unitTemp)
	{
		this->name = unitTemp.name_();
		this->mail = unitTemp.mail_();
		this->pass = unitTemp.pass_();
		this->desc = unitTemp.desc_();
		this->link = unitTemp.link_();
	}

unit_t::~unit_t()
	{

	}

unit_t& unit_t::operator =(unit_t const& unitTemp)
	{
		this->name = unitTemp.name_();
		this->mail = unitTemp.mail_();
		this->pass = unitTemp.pass_() ;
		this->desc = unitTemp.desc_();
		this->link = unitTemp.link_();

		return *this;
	}

	string unit_t::name_() const
	{
		return name;
	}

	string unit_t::mail_() const
	{
		return mail;
	}

	string unit_t::pass_()
	{
		return pass;
	}

	string unit_t::pass_() const
	{
		return pass;
	}

	string unit_t::desc_() const
	{
		return desc;
	}

	string unit_t::link_() const
	{
		return link;
	}

//	bool unit_t::oldPass_() const
//	{
//		return oldPass;
//	}


	void unit_t::generatePassword(int passLength, bool noSymbols)
	{
		srand(time(NULL));
		string numbers = "0123456789";
		string letters = "abcdefghijklmnopqrstuvwxyz";
		string lettersUp = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		string symbols = "%*)?@#$~";

		int N = 4;

		string passwordTemp;
		int randPos;
		bool numbersUsed = false, lettersUsed = false, lettersUpUsed = false, symbolsUsed = false;
		
		if (noSymbols == true)
			N = 3;
		
		Leap1:
			for (int i = 0; i < passLength; i++)
			{
				int j = 1 + rand() % N;
				switch (j)
				{
				case 1:
					randPos = 0 + rand() % 10;
					passwordTemp = passwordTemp + numbers[randPos];
					numbersUsed = true;
					break;
				case 2:
					randPos = 0 + rand() % 26;
					passwordTemp = passwordTemp + letters[randPos];
					lettersUsed = true;
					break;
				case 3:
					randPos = 0 + rand() % 26;
					passwordTemp = passwordTemp + lettersUp[randPos];
					lettersUpUsed = true;
					break;
				case 4: 
					randPos = 0 + rand() % 8;
					passwordTemp = passwordTemp + symbols[randPos];
					symbolsUsed = true;
				default:
					cout << "error while generating pass";
					break;
				}
			}
		
			if (!(numbersUsed == true && lettersUsed == true && lettersUpUsed == true && (symbolsUsed == true || N == 3)))
			{
				numbersUsed = false;
				lettersUsed = false;
				lettersUpUsed = false;
				symbolsUsed = false;
				passwordTemp.erase();
				goto Leap1;
			}

			pass = passwordTemp;
	}

	ostream& unit_t::unitOutput(ostream& stream) const
	{
		stream << endl << "name: " << name << endl;
		if (mail.length() != 0)
			stream << "login: " << mail << endl;
		if (pass.length() != 0)
			stream << "password: " << pass << endl;
		if (desc.length() != 0)
			stream << "description: " << desc << endl;
		if (link.length() != 0)
			stream << "link: " << link << endl;
		return stream;

	}

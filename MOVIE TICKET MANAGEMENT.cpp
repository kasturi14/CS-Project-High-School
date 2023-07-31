#include<iostream.h>
#include<fstream.h>
#include<string.h>
#include<stdlib.h>
#include<iomanip.h>
#include<conio.h>
#include<stdio.h>
#include<process.h>

ofstream ofile("ticket.txt",ios::out);

class HALL
{
	char hname[10];
	int code, hcode,ns;

	public:
	class SHOW           //nested class SHOW within HALL
	{
		char showtime[4];

		public:
		char tcts[200];
		void getshow(int k)
		{
			cout<<"\tShowtime("<<(k+1)<<"):";
			gets(showtime);
			allot_empty();
		}
		char *rs_showtime()
		{
			return showtime;
		}
		void allot_empty();
	}s[4]; //object of SHOW

	void getcode(int c)
	{
		code=c;
	}
	void gethall(int c,int hc,int j);
	void show_hall();
	int rh_code()
	{
		return code;
	}
	int rh_ns()
	{
		return ns;
	}
	int rh_hcode()
	{
		return hcode;
	}
	char *rh_hname()
	{
		return hname;
	}
}h ;              //object of class HALL

class MOVIE
{
	char mname[50];
	int code;
	int per;
	char cat[3];

	public:

	void getmovie(int c)
	{
		code=c;
		cout<<"\tMovie name: ";
		gets(mname);
		cout<<"\n\tDuration of the movie(in minutes): ";
		cin>>per;
		cout<<"\n\tCategory of the movie(U/A or A or U): ";
		gets(cat);
		//if(strcmpi(cat,"u/a")!=0 ||strcmpi(cat,"a")!=0||strcmpi(cat,"u")!=0)
		//{
		  //	cout<<"\n\tWrong category entered. Enter again: ";
		  //	gets(cat);
		//}
		}
		void getcode(int c)
		{
		code=c;
		}
		void modify(int cd)
		{
			char name[30],category[3];int dur;
			cout<<"New movie name(Enter '.' to retain old one):";
			gets(name);
			cout<<"New movie duration(Enter -1 to retain old one):";
			cin>>dur;
			cout<<"New movie category(Enter '.' to retain old one):";
			gets(category);
			code=cd;
			if(strcmp(name,".")!=0)
				strcpy(mname,name);
			if(dur!=-1)
				per=dur;
			if(strcmp(category,".")!=0)
				strcpy(cat,category);
		}
		void display()
		{
			cout<<setw(11)<<code<<setw(15)<<mname<<setw(9)<<per<<setw(20)<<cat<<endl;
		}
		int rm_code()
		{
			return code;
		}
		int rm_per()
		{
			return per;
		}
		char *rm_mname()
		{
			return mname;
		}
		char *rm_cat()
		{
			return cat;
		}
}m; // object of class MOVIE

/********************************************
	INLINE FUNCTIONS
********************************************/

void HALL::SHOW::allot_empty()                     //function to allote the seats of the hall empty
{
	for(int j=0;j<200;j++)
		tcts[j]='E';
}
void HALL::gethall(int c,int hc,int j)
{
	cout<<"\n\tHall name("<<j<<"): ";
	gets(hname);
	code=c;
	hcode=hc;
	cout<<"\n\tNo. of shows: ";
	cin>>ns;
	cout<<"\tShowtime in 12-hr format(AM/PM):\n";
	for(int k=0;k<ns;k++)
		s[k].getshow(k);
}
void HALL::show_hall()
{
	cout<<"Hall Name: "<<hname<<"\t"<<"Hall Code: "<<hcode<<endl;
	cout<<"\nSlot No.:"<<"\t"<<"Showtime;"<<endl;
	for(int i=1;i<=ns;i++)
		cout<<i<<". "<<"\t\t"<<s[i-1].rs_showtime()<<endl;
}



/***********************************************
          Function to add movie
          and its corresponding
			hall details when
          movie.dat file
          already exists.

        add movie for the first time
***************************************/
void add_movie()
{
			 int n2,j,c=0,hc=1;
			 long pos;

          ifstream ifile("movie.dat", ios::binary|ios::in);
          ifile.seekg(0,ios::end);
          pos=ifile.tellg();
          ifile.seekg(pos-sizeof(m));
          if(pos==0)
                 c=1;
          else
          {
                 ifile.read((char*)&m, sizeof(m));
                 ifile.seekg(pos-sizeof(m));
                 c=m.rm_code()+1;
          }
          ifile.close();

          ofstream ofile1("movie.dat",ios::out|ios::binary|ios::app);
          ofstream ofile2("hall.dat",ios::out| ios::binary|ios::app);
          m.getmovie(c);
          ofile1.write((char*)&m,sizeof(m));

          cout<<"\n\tNo. of halls: ";
          cin>>n2;
          for(j=1;j<=n2;j++)
          {
          h.gethall(c,hc,j);
          hc++;
          ofile2.write((char*)&h, sizeof(h));
          }

          ofile1.close();
          ofile2.close();
          cout<<"\n\nPress enter..";
          getch();

}
/***************************************************
        Function to remove movie and
          its corresponding hall details
****************************************************/
void remove_movie()
{
	int cd;
	ifstream ifile1("movie.dat",ios::in|ios::binary);
	ofstream ofile1("tempm.dat",ios::out|ios::binary|ios::app);
   ofstream ofile2("temph.dat",ios::out|ios::binary|ios::app);
		  cout<<"\n\n\t\tEnter movie code to remove: ";
        cin>>cd;
        while(ifile1.read((char*)&m,sizeof(m)))
        {
        if(cd!=m.rm_code())
        {
        ifstream ifile2("hall.dat",ios::in|ios::binary);
        while(ifile2.read((char*)&h,sizeof(h)))
        {
        if(h.rh_code()==m.rm_code())
        {

        ofile2.write((char*)&h,sizeof(h));
        }
        }
        ifile2.close();

        ofile1.write((char*)&m,sizeof(m));
        }
        }
        ifile1.close();
        ofile1.close();
        ofile2.close();
        remove("movie.dat");
        rename("tempm.dat","movie.dat");
        remove("hall.dat");
        rename("temph.dat","hall.dat");
        cout<<"\n\tRecord has been removed successfully. \n\tPress any key to go back to Menu";
        getch();
}
/**************************************************
          Function to deallocate all the seats
***************************************************/

void vacate_seat()
{
	int i;
	ifstream ifile("hall.dat",ios::in|ios::binary);
	ofstream ofile1("temph.dat",ios::out|ios::binary);
	ifile.clear();
	ifile.seekg(0,ios::beg);
	while(ifile.read((char*)&h,sizeof(h)))
	{
		if(ifile.eof())
			break;
		for(i=0;i<h.rh_ns();i++)
			h.s[i].allot_empty();
		ofile1.write((char*)&h,sizeof(h));
	}
	ifile.close();
	ofile1.close();
	remove("hall.dat");
	rename("temph.dat","hall.dat");
	cout<<"All the seats of all the halls for every film have been vacated"<<endl;
   getch();
}

/***************************************************
          Function to print tickets
****************************************************/

void getTicket(int cd,int hc,int sn,int r,int c)
{
	ifstream ifile1("movie.dat",ios::in|ios::binary);
	ifstream ifile2("hall.dat",ios::in|ios::binary);
	while(ifile2.read((char*)&h,sizeof(h)))
	{
		if(ifile2.eof())
			break;
		if((cd==h.rh_code()) &&(hc==h.rh_hcode()))
		{
			ofile<<h.rh_hname()<<endl;
			ofile<<"Showtime: "<<h.s[sn-1].rs_showtime()<<endl;
			ofile<<"Seat Row no.:"<<(r+1)<<" Column no.:"<<(c+1)<<"\t";
			if((r*10+c)<=60)
				ofile<<"Rs 200"<<endl;
			else if((r*10+c)>60 && (r*10+c)<=120)
				ofile<<"Rs 230"<<endl;
			else
				ofile<<"Rs 250"<<endl;
		}
	}
	while(ifile1.read((char*)&m,sizeof(m)))
	{
		if(ifile1.eof())
			break;
		if(cd==m.rm_code())
			ofile<<m.rm_mname()<<endl;
		ofile<<"*********************************"<<endl;
	}
	ifile1.close();
	ifile2.close();
}

/****************************************************************
          Function to change the status of a particular seat
*****************************************************************/

void book(int cd,int hc,int sn,int r,int c)
{
	int pos;
	fstream ifile("hall.dat",ios::in|ios::out|ios::binary);
	while(ifile.read((char*)&h,sizeof(h)))
	{
		if((cd==h.rh_code()) &&(hc==h.rh_hcode()) )
		{
			pos=-1*sizeof(h);
			if(h.s[sn-1].tcts[r*10+c]=='E')
				h.s[sn-1].tcts[r*10+c]='O';
			ifile.seekg(pos,ios::cur);
			ifile.write((char*)&h,sizeof(h));
		}
	}
	ifile.close();
	getTicket(cd,hc,sn,r,c);
}
/****************************************************************
        Function to see the arrangement of seats
*****************************************************************/

void show(int cd,int hc,int sn)
{
	clrscr();
	int i,j,r,c,n,emp=0,flag=0,flag1=0,cost=0;//o1=emp and o=booked
	int k,ctr=0;
	ifstream ifile1("hall.dat",ios::in|ios::binary);
	while(ifile1.read((char*)&h,sizeof(h)))
	{
		if((cd==h.rh_code()) &&(hc==h.rh_hcode()) )
		{
			flag1=1;
			if(h.rh_ns()>=sn)
			{
				flag=1;
				gotoxy(7,1);
				cout<<"  ||=========SCREEN==========||  A-Available seats  B-Booked seats\n\n";
				cout<<"COLUMN->";
				for(j=1;j<=10;j++)
					cout<<" "<<j<<" ";
				cout<<"\n";
				for(i=1;i<=20;i++)
					cout<<"Row:"<<i<<"\n";
				for(i=0;i<20;i++)
				{
					for(j=0;j<10;j++)
					{
						if(h.s[sn-1].tcts[i*10+j]=='E')
						{
							emp++;
							gotoxy(6+3*(j+1),i+4);
							puts(" A ");
						}
						else
						{
							gotoxy(6+3*(j+1),i+4);
							puts("[B]");
						}
					}
					cout<<endl;
				}
				gotoxy(60,8);
				puts("Rows 1-6:Rs 200");
				gotoxy(60,9);
				puts("Rows 7-12:Rs 230");
				gotoxy(60,10);
				puts("Rows 13-20:Rs 250");
				gotoxy(1,24);
			}
			else
			{
				cout<<"This slot number doesnot exist!\n\n\tPress any key to continue..";
				getch();
			}
		}
	}
	ifile1.close();
	if(flag1==0)
	{
		cout<<"This hall code does not belong to any of the halls!\n\n\tPress any key to continue..";
		getch();
	}
	if(flag==1)
	{
		cout<<"Enter number of seats you want to book:"<<endl;
		cin>>n;
		if(n<emp)
		{
			enter:
			if(n==1)
			{
				cout<<"Enter the row:";
				cin>>r;
				cout<<"Enter the column:";
				cin>>c;
			}
			else
			{
				cout<<"Enter the row:";
				cin>>r;
				cout<<"Enter the column:";
				cin>>c;
				r-=1;c-=1;
				ifstream ifile("hall.dat",ios::in|ios::binary);
				while(ifile.read((char*)&h,sizeof(h)))
				{
					if((cd==h.rh_code()) &&(hc==h.rh_hcode()) )
					{
						for(k=0;k<n;k++)
						{
							if(h.s[sn-1].tcts[r*10+c+k]=='E')
								ctr++;
							if(h.s[sn-1].tcts[r*10+c+k]=='O')
							{
								cout<<"OOPS! Enough seats not available!!\n"  ;
								ctr=0;
								goto enter ;
							}
						}
						if(ctr==n)
						{
							for(k=0;k<n;k++)
							{
								book(cd,hc,sn,r,(c+k));
								if((r*10+c+k)<=60)
									cost+=200;
								else if((r*10+c+k)>60 && (r*10+c+k)<=120)
									cost+=230;
								else
									cost+=250;
							}
						}
					}
				}
				ifile.close();
			}
			//printing after booking->->->->
			ifstream file("hall.dat",ios::in|ios::binary);
			while(file.read((char*)&h,sizeof(h)))
			{
				if((cd==h.rh_code()) &&(hc==h.rh_hcode()) )
				{
					clrscr();
					emp=0 ;
					gotoxy(7,1);
					cout<<"  ||=========SCREEN==========||  A-Available seats  B-Booked seats\n\n";
					cout<<"COLUMN->";
					for(j=1;j<=10;j++)
						cout<<" "<<j<<" ";
					cout<<"\n";
					for(i=1;i<=20;i++)
						cout<<"Row:"<<i<<"\n";
					for(i=0;i<20;i++)
					{
						for(j=0;j<10;j++)
						{
							if(h.s[sn-1].tcts[i*10+j]=='E')
							{
								emp++;
								gotoxy(6+3*(j+1),i+4);
								puts(" A ");
							}
							else
							{
								gotoxy(6+3*(j+1),i+4);
								puts("[B]");
							}
						}
						cout<<endl;
					}
					gotoxy(60,8);puts("Rows 1-6:Rs 200");
					gotoxy(60,9);puts("Rows 7-12:Rs 230");
					gotoxy(60,10);puts("Rows 13-20:Rs 250");
					gotoxy(1,24);
				}
        }
        file.close();
		  cout<<"Seat(s) has(have) been successfully booked.\nTotal amount to be paid=Rs "<<cost<<endl;getch();
		}
		else
		{
			cout<<"So many seats are not available. Only "<<emp<<" seats are available.";
			getch();
		}
	}
}
/****************************************************************
        Function to show halls and the other details
          for the chosen movie
*****************************************************************/

void hall_show(int cd)
{
	int hc,i,sn,flag=0;
	clrscr();
	gotoxy(8,1);
	puts("|--------------------|--------------------|--------------------|");
	cout<<endl;
	ifstream ifile("hall.dat",ios::in|ios::binary);
	cout<<"\tHALLS"<<endl;
	while(ifile.read((char*)&h,sizeof(h)))
	{
		if(cd==h.rh_code())
		{
			flag=1;
			cout<<"Hall Name: "<<h.rh_hname()<<"\t"<<"Hall Code: "<<h.rh_hcode()<<endl;
			cout<<"\nSlot No.:"<<"\t"<<"Showtime:"<<endl;
			for(i=1;i<=h.rh_ns();i++)
				cout<<i<<". "<<"\t\t"<<h.s[i-1].rs_showtime()<<endl;
		}
	}
	if(flag==1)
	{
		cout<<"Enter hall code:";
		cin>>hc;
		cout<<"Enter the slot no.:";
		cin>>sn;
		ifile.close();
		show(cd,hc,sn);
	}
	else
		cout<<"This movie code does not belong to any movie"<<endl;
}

void main()
{

	clrscr();
	int ch,cd,choice;
	long pos;
	char found='f';
	fstream ifile;
	ifile.open("movie.dat", ios::in|ios::out|ios::binary);
	ifile.seekg(0,ios::beg);
	gotoxy(30,7);puts(" !!...WELCOME...!!");
	gotoxy(27,9);cout<<"========================";
	gotoxy(27,10);cout<<"`````||````||````||`````  ";
	gotoxy(29,11);cout<<"   ||    ||    || ";
	gotoxy(27,12);cout<<",,,,,||,,,,||,,,,||,,,,, ";
	gotoxy(27,13);cout<<"========================";
	gotoxy(28,15);cout<<"MOVIE TICKET MANAGEMENT";
	cout<<"\n\n\n\nPress any key to continue..";
	getch();
	do
	{
		clrscr();
		cout<<"\n\n\t\t\t...MAIN MENU...\n\n\t\t1.Movie details\n \n\t\t2.Book a ticket\n\n\t\t3.Display all the movie details\n\n\t\t4.Exit\n\n\t\t\Enter your choice:";
		cin>>ch;
		switch(ch)
		{
			case 1:
			ifile.open("movie.dat", ios::in|ios::out|ios::binary);
			check:
			clrscr();
			cout<<"\n\n\t\t\tMenu\n\t\t\t~~~~~\n\t\t
			1.Add a movie \n\n\t\t
			2.Modify details of a movie\n\n\t\t
			3.Remove a movie\n\n\t\t
			4.Vacate all the seats of all halls\n\n\t\t
			5.Return to main menu\n\n\t\t
			Enter your choice:";
			cin>>choice;
			cout<<endl;
			if(choice==1)
			{
				clrscr();
				gotoxy(27,3);
				cout<<"ADD A MOVIE\n";
				gotoxy(27,4);
				cout<<"-----------\n";
				add_movie();
			}
			else if(choice==2)
			{
            clrscr();
				gotoxy(27,3);
				cout<<"MODIFY A MOVIE\n";
				gotoxy(27,4);
				cout<<"--------------\n";
				cout<<"\nMovie code\tMovie name\tDuration\tCategory"<<endl;
				cout<<"----------\t----------\t--------\t--------\n";
				ifile.clear();
				ifile.seekg(0,ios::beg);
				while(ifile.read((char*)&m,sizeof(m)))
				{
					if(ifile.eof())
					{
						cout<<"No movies available!";
						break;
					}
					m.display();
				}
				ifile.close();
				ifile.open("movie.dat", ios::in|ios::out|ios::binary);
				cout<<"\n\nEnter movie code:";
				cin>>cd;
				ifile.clear();
				ifile.seekg(0);
				pos=0;
				while(ifile.read((char*)&m,sizeof(m)))
				{
					if(ifile.eof())
						break;
					pos=ifile.tellg();
					if(cd==m.rm_code())
					{
						cout<<"\nMovie code\tMovie name\tDuration\tCategory\n";
						cout<<"----------\t----------\t--------\t--------\n";
						m.display();
						cout<<"Enter new details.\n\n";
						m.modify(cd);
						ifile.seekg(pos-sizeof(m));
						ifile.write((char*)&m,sizeof(m));
						found='t';
						break;
					}
				}
				if(found=='f')
					cout<<"\nRecord not found!\n";
			}

			else if(choice==3)
			{
         	clrscr();
				gotoxy(27,3);
				cout<<"REMOVE A MOVIE\n";
				gotoxy(27,4);
				cout<<"--------------\n";
				cout<<"\t\t\t    MENU"<<endl;
				cout<<"\t\t\t    ----\n";
				cout<<"\t\tMovie code\tMovie name"<<endl;
				cout<<"\t\t----------\t----------\n";
				ifile.clear();
				ifile.seekg(0);
				while(ifile.read((char*)&m,sizeof(m)))
				{
					if(ifile.eof())
					{
						cout<<"No movies available!";
						break;
					}
					cout<<"\t\t"<<m.rm_code()<<"\t\t"<<m.rm_mname()<<endl;
				}
				remove_movie();
			}

			else if(choice==4)
				vacate_seat();
			else if(choice==5)
				break;
			else
				cout<<"Incorrect choice!";
			ifile.close();
			goto check;

			case 2:
			ifile.open("movie.dat", ios::in|ios::out|ios::binary);
			clrscr();
			gotoxy(27,3);
			cout<<" BOOK A MOVIE\n";
			gotoxy(27,4);
			cout<<"--------------\n";
			cout<<"\t\t\t\tMENU"<<endl;
         cout<<"\t\t\t\t----\n";
			cout<<"Movie code\tMovie name\tDuration\tCategory"<<endl;
			cout<<"----------\t----------\t--------\t--------\n";
			ifile.clear();
			ifile.seekg(0);
			while(ifile.read((char*)&m,sizeof(m)))
			{
				if(ifile.eof())
				{
					cout<<"No movies available!";
					break;
				}
				m.display();
			}
			cout<<"\n\nEnter movie code:";
			cin>>cd;
			hall_show(cd);
			ifile.close();
			break;

			case 3:
			ifile.open("movie.dat", ios::in|ios::out|ios::binary);
			clrscr();
			cout<<"\n\t\t\tMOVIE LIST";
         cout<<"\n\t\t\t----------\n";
			cout<<"\nMovie code\tMovie name\tDuration\tCategory"<<endl;
			cout<<"----------\t----------\t--------\t--------\n";
			ifile.clear();
			ifile.seekg(0);
			while(ifile.read((char*)&m,sizeof(m)))
			{
				if(ifile.eof())
				{
					cout<<"No movies available!";
					break;
				}
				m.display();
			}
			cout<<"\n\nPress any key to continue...";
			getch();
			ifile.close();
			break;

			case 4:
			clrscr();
			gotoxy(26,10);
			cout<<"THANK YOU FOR USING THE SYSTEM !";
			exit(0);
			break;

			default: cout<<"Oops!! Try again";
		}
	}while(ch>=1 &&ch<=4);
	ofile.close();
}
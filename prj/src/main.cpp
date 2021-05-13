/*!
 * \file
 * \brief Przykład animacji przelotu drona
 *
 * Przykład ten ma tylko i wyłącznie za zadanie pokazanie efektów animacji lotu
 * drona. Kod w nim zawarty nie jest pisany w sposób obiektowo zorientowany.
 * Dlatego też nie należy wzorować się na nim.
 * Niemniej można się wzorować na ciągu operacji, które należy wykonać,
 * aby otrzymać właściwą animację.
 */



#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cassert>
#include <unistd.h>
#include "lacze_do_gnuplota.hh"

using namespace std;





/*

#define ILOSC_WIERZCHOLKOW_GRANIATOSLUPA  6
#define PROMIEN_PODSTAWY                  10
#define GRUBOSC_ROTORA                    5.0
*/


#define ILOSC_WIERZ_LINII_TWORZACEJ            4

#define PLIK_WZORCOWEGO_SZESCIANU       "bryly_wzorcowe/szescian.dat"
#define PLIK_WZORCOWEGO_GRANIASTOSLUPA6 "bryly_wzorcowe/graniastoslup6.dat"
#define PLIK_KORPUSU_DRONA1             "dat/korpus_drana1.dat"





/*!
 * \brief Przelicza wartość kąta ze stopni na radiany.
 *
 * Przelicza wartość kąta ze stopni na radiany.
 * \param[in] Kat_st - wartość kąta wyrażona w stopniach.
 * \return Wartość przeliczonego kąta wyrażona w radianach.
 */
double StopnieNaRadiany(double Kat_st)
{
  return Kat_st*M_PI/180;
}



/*!
 * \brief Dokonuje obrotu wokół osi OZ.
 *
 * Transformuje współrzędne \p x i \p y dokonując obrót o zadany kąt.
 * \param[in] KatObrotu_st - kąt, o który ma być zrealizowany obrót.
 * \param[in,out] x_pkt - współrzędna x-owa punktu, która zostaje przetransformowana.
 * \param[in,out] y_pkt - współrzędna y-owa punktu, która zostaje przetransformowana.
 */
void  ObrocWzgledemOsiOZ( double  KatObrotu_st, double  &x_pkt, double  &y_pkt )
{
  double Kat_rad = StopnieNaRadiany(KatObrotu_st);
  double sn = sin(Kat_rad), cn = cos(Kat_rad);
  double x_nowe = x_pkt*cn-y_pkt*sn;
  double y_nowe = x_pkt*sn+y_pkt*cn;
  x_pkt = x_nowe;  y_pkt = y_nowe;
}




/*!
 * \brief Transformuje współrzędne pojedynczego punktu.
 * 
 * Transformuje współrzędne pojedynczego punktu poprzez ich przeskalowanie i 
 * przesunięcie o zdany wektor translacji. Współczynniki skalowania jak też
 * współrzędne wektora translacji są przekazywane przez parametry tej funkcji.
 *
 * \param[in] KatObrotu_st - kąt, o jaki ma być dokonany obrót. Wartość kąta wyrażona
 *                           jest w stopniach.
 * \param[in,out]  x_wierz - współrzędna x-owa punktu, która ma zostać poddana transformacji.
 * \param[in,out]  y_wierz - współrzędna y-owa punktu, która ma zostać poddana transformacji.
 * \param[in,out]  z_wierz - współrzędna z-owa punktu, która ma zostać poddana transformacji.
 * \param[in]      sx_skala - współczynnik skalowania wzdłuż osi OX.
 * \param[in]      sy_skala - współczynnik skalowania wzdłuż osi OY.
 * \param[in]      sz_skala - współczynnik skalowania wzdłuż osi OZ.
 * \param[in]      tx_trans - współrzędna x-owa wektora translacji.
 * \param[in]      ty_trans - współrzędna y-owa wektora translacji.
 * \param[in]      tz_trans - współrzędna z-owa wektora translacji.
 */
void TransformujWspolrzednePunktu(
				  double  KatObrotu_st,
				  double  &x_wierz, double  &y_wierz, double  &z_wierz,
				  double  sx_skala, double  sy_skala, double  sz_skala,
		                  double  tx_trans, double  ty_trans, double  tz_trans
				 )
{
  ObrocWzgledemOsiOZ(KatObrotu_st,x_wierz,y_wierz);
  x_wierz = x_wierz * sx_skala + tx_trans;
  y_wierz = y_wierz * sy_skala + ty_trans;
  z_wierz = z_wierz * sz_skala + tz_trans;  
}



/*!
 * \brief  Transformuje całą bryłę.
 *
 *  Czyta ze strumienia wejściowego współrzędne wierzchołków bryły, dokonuje 
 *  ich transformacji (skalowanie, obrót i translacja)
 *  Zapisuje do strumienia współrzędne wierzchołków przetransformowanej bryły
 *  w ten sposób, aby mogły korpus mógł być poprawnie narysowany przez program gnuplot.
 *  \param[in,out] StrmWe_DaneWejsciowe - strumień zawierający opis bryły, która ma podlegać
 *                                        transformacji. Współrzędne z tego strumienia są
 *                                        tylko czytane.
 *  \param[in,out] StrmWy_DaneWynikowe - strumień zawierający opis bryły po dokonanej tranformacji.
 * \param[in] KatObrotu_st - kąt, o jaki ma być dokonany obrót. Wartość kąta wyrażona
 *                           jest w stopniach.
 * \param[in]      sx_skala - współczynnik skalowania wzdłuż osi OX.
 * \param[in]      sy_skala - współczynnik skalowania wzdłuż osi OY.
 * \param[in]      sz_skala - współczynnik skalowania wzdłuż osi OZ.
 * \param[in]      tx_trans - współrzędna x-owa wektora translacji.
 * \param[in]      ty_trans - współrzędna y-owa wektora translacji.
 * \param[in]      tz_trans - współrzędna z-owa wektora translacji.
 *
 * \retval true - gdy operacja powiedzie się,
 * \retval false - gdy operacja nie powiedzie się z powodu
 *                 niepowodzenia odczytu lub zapisu do strumienia.
 */
bool TworzOpisPrzetransformowanejBryly(
			   istream   &StrmWe_DaneWejsciowe,
			   ostream   &StrmWy_DaneWynikowe,
			   double KatObrotu_st,
                           double sx_skala, double sy_skala, double sz_skala,
		           double tx_trans, double ty_trans, double tz_trans
		         )
{
  assert(StrmWe_DaneWejsciowe.good());
  assert(StrmWy_DaneWynikowe.good());

  double  x_wierz, y_wierz, z_wierz;

  StrmWe_DaneWejsciowe >> x_wierz >> y_wierz >> z_wierz;
  while (!StrmWe_DaneWejsciowe.fail()) {

    for (unsigned int IloscWierzcholkow = 0; IloscWierzcholkow < ILOSC_WIERZ_LINII_TWORZACEJ;
	 ++IloscWierzcholkow) {

      TransformujWspolrzednePunktu( KatObrotu_st,
				    x_wierz, y_wierz, z_wierz,
                                    sx_skala, sy_skala, sz_skala,
				    tx_trans, ty_trans, tz_trans
			          );
      StrmWy_DaneWynikowe << x_wierz << " " << y_wierz << " " << z_wierz << endl;
      StrmWe_DaneWejsciowe >> x_wierz >> y_wierz >> z_wierz;
      
      assert(IloscWierzcholkow == ILOSC_WIERZ_LINII_TWORZACEJ-1 || !StrmWe_DaneWejsciowe.fail());
    }

    StrmWy_DaneWynikowe << endl;
  }
  return !StrmWy_DaneWynikowe.fail();
}




/*!
 * \brief  Transformuje całą bryłę.
 *
 *  Czyta z pliku wejściowego współrzędne wierzchołków bryły, dokonuje 
 *  ich transformacji (skalowanie, obrót i translacja)
 *  Zapisuje do drugiego pliku współrzędne wierzchołków przetransformowanej bryły
 *  w ten sposób, aby mogły korpus mógł być poprawnie narysowany przez program gnuplot.
 *  \param[in,out] NazwaPliku_DaneWejsciowe - nazwa pliku zawierający opis bryły, która ma podlegać
 *                                        transformacji. Współrzędne z tego strumienia są
 *                                        tylko czytane.
 *  \param[in,out] NazwaPliku_DaneWynikowe - nazwa pliku, który ma zawierać opis bryły
 *                                         po dokonanej tranformacji.
 * \param[in] KatObrotu_st - kąt, o jaki ma być dokonany obrót. Wartość kąta wyrażona
 *                           jest w stopniach.
 * \param[in]      sx_skala - współczynnik skalowania wzdłuż osi OX.
 * \param[in]      sy_skala - współczynnik skalowania wzdłuż osi OY.
 * \param[in]      sz_skala - współczynnik skalowania wzdłuż osi OZ.
 * \param[in]      tx_trans - współrzędna x-owa wektora translacji.
 * \param[in]      ty_trans - współrzędna y-owa wektora translacji.
 * \param[in]      tz_trans - współrzędna z-owa wektora translacji.
 *
 * \retval true - gdy operacja powiedzie się,
 * \retval false - gdy operacja nie powiedzie się z powodu braku możliwości 
 *                 otwarcia jednego z plików lub niepowodzenia odczytu lub zapisu do pliku.
 */
bool TworzOpisPrzetranformowanejBryly(
		      const char*   NazwaPliku_DaneWejsciowe,
  		      const char*   NazwaPliku_DaneWynikowe,
		      double KatObrotu_st,
                      double sx_skala, double sy_skala, double sz_skala,
		      double tx_trans, double ty_trans, double tz_trans
		     )
{ 
  ifstream  Plik_BrylaWzorcowa(NazwaPliku_DaneWejsciowe);
  ofstream  Plik_KorpusuDrona(NazwaPliku_DaneWynikowe);

  
  if (!Plik_BrylaWzorcowa.is_open()) {
    cerr << endl
	 << " Blad otwarcia do odczytu pliku: " << NazwaPliku_DaneWejsciowe << endl
	 << endl;
    return false;
  }

  if (!Plik_KorpusuDrona.is_open()) {
    cerr << endl
	 << " Blad otwarcia do odczytu pliku: " << NazwaPliku_DaneWynikowe << endl
	 << endl;
    return false;
  }
  
  return TworzOpisPrzetransformowanejBryly(
				Plik_BrylaWzorcowa,
				Plik_KorpusuDrona,
				KatObrotu_st,
			        sx_skala, sy_skala, sz_skala,
			        tx_trans, ty_trans, tz_trans
			      );
}







/*!
 * \brief Tworzy plik z wierzchołkami były modelującej rotor drona.
 *
 * Tworzy plik, w którym zostają zapise współrzędne wierzchołków
 * tworzących pojedynczy rotor drona. Zapisane one są w ten sposób, aby program gnuplot
 * mógł wyświetlić graniastosłubp Tworzona bryła zostaje obrócona
 * względem swojej pierwotnej orientacji o zadany kąt oraz przesunięta o wektor
 * translacji, którego współrzędne podane są jako parametry tej funkcji.
 *
 * \param[in] NazwPliku_RotorWDronie - nazwa pliku, w którym zostaną zapisane współrzędne
 *                                       wierzchołków bryły tworzącej rotor drona.
 * \param[in] KatObrotuRotora_st - kąt obrotu korpusu. Jego wartość wyrażona jest w stopniach.
 * \param[in] tx_trans - x-owa współrzędna wektora translacji.
 * \param[in] ty_trans - y-owa współrzędna wektora translacji.
 * \param[in] tz_trans - z-owa współrzędna wektora translacji.
 *
 * \retval true - gdy operacja powiedzie się,
 * \retval false - gdy operacja nie powiedzie się z powodu braku możliwości 
 *                 otwarcia jednego z plików lub niepowodzenia odczytu lub zapisu do pliku.
 */
bool TworzRotor(const char*  NazwPliku_RotorWDronie,
                double    KatObrotuRotora_st,
		double tx_trans, double ty_trans, double tz_trans
	       )
{
#define  SKALA_ROTORA  8,8,2  
 return TworzOpisPrzetranformowanejBryly(PLIK_WZORCOWEGO_GRANIASTOSLUPA6,
                                        NazwPliku_RotorWDronie,
					KatObrotuRotora_st,
				        SKALA_ROTORA,
					tx_trans, ty_trans, tz_trans);
}



/*!
 * \brief Tworzy plik z wierzchołkami były modelującej korpus drona.
 *
 * Tworzy plik, w którym zostają zapise współrzędne wierzchołków
 * tworzących korpus drona. Zapisane one są w ten sposób, aby program gnuplot
 * mógł wyświetlić prostopadłościan. Tworzona bryła zostaje obrócona
 * względem swojej pierwotnej orientacji o zadany kąt oraz przesunięta o wektor
 * translacji, którego współrzędne podane są jako parametry tej funkcji.
 *
 * \param[in] NazwPliku_KorpusuWDronie - nazwa pliku, w którym zostaną zapisane współrzędne
 *                                       wierzchołków bryły tworzącej korpus drona.
 * \param[in] KatObrotuKorpusu_st - kąt obrotu korpusu. Jego wartość wyrażona jest w stopniach.
 * \param[in] tx_trans - x-owa współrzędna wektora translacji.
 * \param[in] ty_trans - y-owa współrzędna wektora translacji.
 * \param[in] tz_trans - z-owa współrzędna wektora translacji.
 *
 * \retval true - gdy operacja powiedzie się,
 * \retval false - gdy operacja nie powiedzie się z powodu braku możliwości 
 *                 otwarcia jednego z plików lub niepowodzenia odczytu lub zapisu do pliku. 
 */
bool TworzKorpus(const char*  NazwPliku_KorpusuWDronie,
                 double       KatObrotuKorpusu_st,
		 double tx_trans, double ty_trans, double tz_trans
	        )
{
#define  SKALA_KORPUSU  10,8,4
  
 return TworzOpisPrzetranformowanejBryly(PLIK_WZORCOWEGO_SZESCIANU,
                                        NazwPliku_KorpusuWDronie,
					KatObrotuKorpusu_st,
				        SKALA_KORPUSU,
				        tx_trans, ty_trans, tz_trans);
}




#define PLIK_ROBOCZY__DRON1_KORPUS  "dat/PlikRoboczy_Dron1_Korpus.dat"
#define PLIK_ROBOCZY__DRON1_ROTOR1  "dat/PlikRoboczy_Dron1_Rotor1.dat"
#define PLIK_ROBOCZY__DRON1_ROTOR2  "dat/PlikRoboczy_Dron1_Rotor2.dat"
#define PLIK_ROBOCZY__DRON1_ROTOR3  "dat/PlikRoboczy_Dron1_Rotor3.dat"
#define PLIK_ROBOCZY__DRON1_ROTOR4  "dat/PlikRoboczy_Dron1_Rotor4.dat"

#define PLIK_WLASCIWY__DRON1_KORPUS  "dat/PlikWlasciwy_Dron1_Korpus.dat"
#define PLIK_WLASCIWY__DRON1_ROTOR1  "dat/PlikWlasciwy_Dron1_Rotor1.dat"
#define PLIK_WLASCIWY__DRON1_ROTOR2  "dat/PlikWlasciwy_Dron1_Rotor2.dat"
#define PLIK_WLASCIWY__DRON1_ROTOR3  "dat/PlikWlasciwy_Dron1_Rotor3.dat"
#define PLIK_WLASCIWY__DRON1_ROTOR4  "dat/PlikWlasciwy_Dron1_Rotor4.dat"


/*!
 * \brief Tworzy pliki pozwalające na wizualizację drona.
 * 
 * Zapisuje do odpwiednich plików opisy brył tworzących wizualizację drona.
 *
 * \retval true - gdy operacja powiedzie się,
 * \retval false - gdy operacja nie powiedzie się z powodu braku możliwości 
 *                 otwarcia jednego z plików lub niepowodzenia odczytu lub zapisu do pliku.
 */
bool TworzDrona()
{
  static double KatObrotuRotora_st = 0; 
#define TRANSLACJA_ROTOR_LEWY_PRZEDNI   5,4,5
#define TRANSLACJA_ROTOR_PRAWY_PRZEDNI  5,-4,5
#define TRANSLACJA_ROTOR_LEWY_TYLNY    -5,4,5
#define TRANSLACJA_ROTOR_PRAWY_TYLNY   -5,-4,5  

  //----------------------------------------------------------
  // Korpus drona jest podniesiony o 4. Dzięki temu początek układu
  // współrzędnych drona jest na środku dolnej podstawy korpusu.
  //
  if (!TworzKorpus(PLIK_ROBOCZY__DRON1_KORPUS,0,0,0,2)) return false;

  if (!TworzRotor(PLIK_ROBOCZY__DRON1_ROTOR1,
		    KatObrotuRotora_st,TRANSLACJA_ROTOR_LEWY_PRZEDNI)) return false;
  if (!TworzRotor(PLIK_ROBOCZY__DRON1_ROTOR2,
		    -KatObrotuRotora_st,TRANSLACJA_ROTOR_PRAWY_PRZEDNI)) return false;
  if (!TworzRotor(PLIK_ROBOCZY__DRON1_ROTOR3,
		    -KatObrotuRotora_st,TRANSLACJA_ROTOR_LEWY_TYLNY)) return false;
  if (!TworzRotor(PLIK_ROBOCZY__DRON1_ROTOR4,
		     KatObrotuRotora_st,TRANSLACJA_ROTOR_PRAWY_TYLNY)) return false;
#define SKOK_OBROTU_ROTOROW_stopnie  10
  KatObrotuRotora_st += SKOK_OBROTU_ROTOROW_stopnie;
  return true;
}



/*!
 * \brief Ustawia dron w żądanej pozycji i orientacji.
 *
 * Dron zostaje przeniesiony do wskazanej pozycji. Jego orientacja zostaje zmieniona
 * zgodnie z zadanym kątem.
 *
 * \param[in] KatOrDrona_st - kąta orientacji drona. Jest to kąt względem osi OZ.
 *                            Wartość orientacji drona wyrażona jest w stopniach.
 * \param[in] x_PozDrona - x-owa współrzędna pozycji drona.
 * \param[in] y_PozDrona - y-owa współrzędna pozycji drona.
 * \param[in] z_PozDrona - z-owa współrzędna pozycji drona.
 *
 * \retval true - gdy operacja powiedzie się,
 * \retval false - w przypadku przeciwnym.
 *                 Może się to zdarzyć, gdy niepowiedzie się otwarcie jednego z plików,
 *                 z których korzysta ta funkcja,
 *                 lub niepowodzenia odczytu lub zapisu do wspomnianych plików.
 */
bool PrzemiescDrona(double KatOrDrona_st, double x_PozDrona, double y_PozDrona, double z_PozDrona)
{
#define SKALA_JEDNOSTKOWA  1,1,1
  
  if (!TworzDrona()) return false;
  //--------------------------------------------------
  // Transformacja elementów drona do właściwej pozycji
  //
  const char *NazwyPlikowRoboczych[] = { PLIK_ROBOCZY__DRON1_KORPUS, PLIK_ROBOCZY__DRON1_ROTOR1,
					 PLIK_ROBOCZY__DRON1_ROTOR2, PLIK_ROBOCZY__DRON1_ROTOR3,
					 PLIK_ROBOCZY__DRON1_ROTOR4, nullptr
                                        };
  const char *NazwyPlikowWlasciwych[] = { PLIK_WLASCIWY__DRON1_KORPUS, PLIK_WLASCIWY__DRON1_ROTOR1,
					 PLIK_WLASCIWY__DRON1_ROTOR2, PLIK_WLASCIWY__DRON1_ROTOR3,
					 PLIK_WLASCIWY__DRON1_ROTOR4, nullptr
                                        };
  
  assert(sizeof(NazwyPlikowRoboczych) == sizeof(NazwyPlikowWlasciwych));

  for (unsigned int Idx = 0; NazwyPlikowRoboczych[Idx] != nullptr; ++Idx) {
    if (!TworzOpisPrzetranformowanejBryly(NazwyPlikowRoboczych[Idx],
					 NazwyPlikowWlasciwych[Idx],
					 KatOrDrona_st,
				         SKALA_JEDNOSTKOWA,
				         x_PozDrona, y_PozDrona, z_PozDrona)) return false;
  }
  return true;
}



#define PLIK_TRASY_PRZELOTU "dat/trasa_przelotu.dat"

/*!
 * \brief Dodaje trasę przelotu.
 *
 * Tworzony jest plik z trasę przelotu w postaci łamanej.
 * Nazwa pliku zostaje dodana do łącza do gnuplota.
 * Dzięki temu dane zawarte w tym pliku zostaną użyte przez
 * gnuplota do narysowania łamanej.
 *
 * \retval true - gdy operacja powiedzie się,
 * \retval false - w przypadku przeciwnym.
 *                 Może się to zdarzyć, gdy niepowiedzie się otwarcie plików,
 *                 do którego zapisywane są współrzędne łamanej modelującej ścieżkę
 *                 przelotu drona, lub niepowodzenia zapisu do tego pliku.
 */
bool DodajTrasePrzelotu(PzG::LaczeDoGNUPlota &Lacze)
{
  ofstream  StrmWy(PLIK_TRASY_PRZELOTU);

  if (!StrmWy.is_open()) {
    cerr << endl
	 << " Nie mozna otworzyc do zapisu pliku: " << PLIK_TRASY_PRZELOTU << endl
	 << endl;
    return false;
  }

  StrmWy << "20  20   0" << endl
	 << "20  20  80" << endl
	 << "150 150 80" << endl
	 << "150 150  0" << endl;
  Lacze.DodajNazwePliku(PLIK_TRASY_PRZELOTU);
  return !StrmWy.fail();
}


/*!
 * \brief Anicja przelotu drona.
 *
 * Animacja przelotu drona.
 * Animacja ta jest robiona na "sztywno"
 *
 * \retval true - gdy operacja powiedzie się,
 * \retval false - w przypadku przeciwnym.
 *                 Może się to zdarzyć, gdy niepowiedzie się otwarcie jednego z plików,
 *                 z których korzysta ta funkcja,
 *                 lub niepowodzenia odczytu lub zapisu do wspomnianych plików.
 */
bool AnimacjaLotuDrona(PzG::LaczeDoGNUPlota &Lacze)
{
  double x_dron = 20, y_dron = 20, z_dron = 0;
  double KatOr_st = 0;

  //-------------------------------------
  // Wznoszenie ...
  //
  cout << endl << "Wznoszenie ... " << endl;
  for (; z_dron <= 80; z_dron += 2) {
    if (!PrzemiescDrona(KatOr_st,x_dron,y_dron,z_dron)) return false;
    usleep(100000); // 0.1 ms
    Lacze.Rysuj();
  }
  z_dron -= 2;
  
  cout << "Zmiana orientacji ... " << endl;
  for (; KatOr_st <= 45; KatOr_st += 5) {
    if (!PrzemiescDrona(KatOr_st,x_dron,y_dron,z_dron)) return false;
    usleep(100000);
    Lacze.Rysuj();
  }
  KatOr_st -= 5;

  //-------------------------------------
  // Lot do przodu ...
  //  
  cout << "Lot do przodu ... " << endl;
  for (; x_dron <= 150; x_dron += 1, y_dron += 1) {
    if (!PrzemiescDrona(KatOr_st,x_dron,y_dron,z_dron)) return false;
    usleep(100000);
    Lacze.Rysuj();
  }  
  x_dron -= 1, y_dron -= 1;


  //-------------------------------------
  // Opadanie ...
  //
  cout << "Opadanie ... " << endl;
  for (; z_dron >= 0; z_dron -= 2) {
    if (!PrzemiescDrona(KatOr_st,x_dron,y_dron,z_dron)) return false;
    usleep(100000); // 0.1 ms
    Lacze.Rysuj();
  }

  return true;
}





int main()
{
  PzG::LaczeDoGNUPlota  Lacze;

  Lacze.DodajNazwePliku("bryly_wzorcowe/plaszczyzna.dat");
  Lacze.DodajNazwePliku(PLIK_WLASCIWY__DRON1_KORPUS);
  Lacze.DodajNazwePliku(PLIK_WLASCIWY__DRON1_ROTOR1);
  Lacze.DodajNazwePliku(PLIK_WLASCIWY__DRON1_ROTOR2);
  Lacze.DodajNazwePliku(PLIK_WLASCIWY__DRON1_ROTOR3);
  Lacze.DodajNazwePliku(PLIK_WLASCIWY__DRON1_ROTOR4);

  Lacze.ZmienTrybRys(PzG::TR_3D);
  Lacze.Inicjalizuj();  // Tutaj startuje gnuplot.

  Lacze.UstawZakresX(0, 200);
  Lacze.UstawZakresY(0, 200);
  Lacze.UstawZakresZ(0, 120);


  Lacze.UstawRotacjeXZ(64,65); // Tutaj ustawiany jest widok

  if (!PrzemiescDrona(0,20,20,0)) return 0;


  Lacze.Rysuj();        // Teraz powinno pojawic sie okienko gnuplota
                        // z rysunkiem, o ile istnieje plik "prostopadloscian1.pow"
  cout << "Nacisnij ENTER, aby pokazac sciezke przelotu drona " << flush;
  cin.ignore(10000,'\n');


  if (!DodajTrasePrzelotu(Lacze)) return 1;
  Lacze.Rysuj();


  cout << "Nacisnij ENTER, aby wykonac animacje lotu drona " << flush;
  cin.ignore(10000,'\n');
  if (!AnimacjaLotuDrona(Lacze)) return 1;

  cout << endl << "Nacisnij ENTER, aby usunac sciezke ... " << flush;
  cin.ignore(10000,'\n');

  Lacze.UsunNazwePliku(PLIK_TRASY_PRZELOTU);
  Lacze.Rysuj();
  
  cout << "Nacisnij ENTER, aby zakonczyc ... " << flush;
  cin.ignore(10000,'\n');
  
}

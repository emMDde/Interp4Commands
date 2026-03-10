#include <xercesc/util/PlatformUtils.hpp>
#include "XMLInterp4Config.hh"
#include <cassert>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <stdexcept>



using namespace std;


/*!
 * Konstruktor klasy. Tutaj należy zainicjalizować wszystkie
 * dodatkowe pola.
 */
XMLInterp4Config::XMLInterp4Config(Configuration &rConfig) : _rConfig(rConfig)
{}


/*!
 * Metoda wywoływana jest bezpośrednio przed rozpoczęciem
 * przetwarzana dokumentu XML.
 */
void XMLInterp4Config::startDocument()
{
  cout << "*** Rozpoczecie przetwarzania dokumentu XML." << endl;
}


/*!
 * Metoda wywoływana jest bezpośrednio po zakończeniu
 * przetwarzana dokumentu XML.
 */
void XMLInterp4Config::endDocument()
{
  cout << "=== Koniec przetwarzania dokumentu XML." << endl;
}





/*!
 * Analizuje atrybuty elementu XML \p "Lib" i odpowiednio je interpretuje.
 * \param[in] rAttrs - atrybuty elementu XML \p "Lib".
 */
void XMLInterp4Config::ProcessLibAttrs(const xercesc::Attributes  &rAttrs)
{
 if(rAttrs.getLength() != 1) throw invalid_argument("Zla ilosc atrybutow dla \"Lib\"");
 
 char* sParamName = xercesc::XMLString::transcode(rAttrs.getQName(0));
 if (strcmp(sParamName,"Name"))
 {
  xercesc::XMLString::release(&sParamName);
  throw invalid_argument("Zla nazwa atrybutu dla Lib");
 }    
 xercesc::XMLString::release(&sParamName);

 XMLSize_t  Size = 0;
 char* sLibName = xercesc::XMLString::transcode(rAttrs.getValue(Size));
 cout << "  Nazwa biblioteki: " << sLibName << endl;
 string LibName=sLibName;
 xercesc::XMLString::release(&sLibName);

 _rConfig._LibraryList.push_back(LibName);
}


/*!
 * Analizuje atrybuty. Sprawdza czy ich nazwy są poprawne. Jeśli tak,
 * to pobiera wartości atrybutów (w postaci napisów) i przekazuje ...
 * \param[in] rAttrs - atrybuty elementu XML \p "Cube".
 */
void XMLInterp4Config::ProcessCubeAttrs(const xercesc::Attributes &rAttrs)
{
  XMLSize_t xmlSize=rAttrs.getLength();
  if(xmlSize < 1) throw invalid_argument("Zla ilosc atrybutow dla \"Cube\"");

  Cuboid *pCuboid=new Cuboid();
  bool isName=false;
  bool isCorrect=true;
  string ErrorMsg;
  char bin;

  for(XMLSize_t Index=0; Index < xmlSize; ++Index) 
  {
    char *sAttrName=xercesc::XMLString::transcode(rAttrs.getQName(Index));
    char *sAttrValue=xercesc::XMLString::transcode(rAttrs.getValue(Index));
    
    istringstream IStrm(sAttrValue);
    Vector3D Value; 

    if(strcmp(sAttrName, "Name") == 0) {
      pCuboid->SetName(sAttrValue);
      isName=true;
    }
    else if(strcmp(sAttrName, "Scale") == 0) {
      if(!(IStrm >> Value) || (IStrm >> bin)) {
        isCorrect=false;
        ErrorMsg="Blad parsowania atrybutu Scale!";
      } 
      else pCuboid->SetScale(Value);
    }
    else if(strcmp(sAttrName, "Shift") == 0) {
      if(!(IStrm >> Value) || (IStrm >> bin)) {
        isCorrect=false;
        ErrorMsg="Blad parsowania atrybutu Shift!";
      } 
      else pCuboid->SetShift(Value);
    }
    else if(strcmp(sAttrName, "RGB") == 0) {
      VectorRGB Color;
      if(!(IStrm >> Color) || (IStrm >> bin) || (Color[0]>255 || Color[1]>255 || Color[2]>255)) {
        isCorrect=false;
        ErrorMsg="Blad parsowania atrybutu RGB!";
      }
      else pCuboid->SetColor(Color);
    }
    else if(strcmp(sAttrName, "Trans_m") == 0) {
      if(!(IStrm >> Value) || (IStrm >> bin)) {
        isCorrect=false;
        ErrorMsg="Blad parsowania atrybutu Trans_m!";
      } 
      else pCuboid->SetTrans_m(Value);
    }
    else if(strcmp(sAttrName, "RotXYZ_deg") == 0) {
      if(!(IStrm >> Value) || (IStrm >> bin)) {
        isCorrect=false;
        ErrorMsg="Blad parsowania atrybutu RotXYZ_deg!";
      } 
      else {
        pCuboid->SetAng_Roll_deg(Value[0]); 
        pCuboid->SetAng_Pitch_deg(Value[1]);
        pCuboid->SetAng_Yaw_deg(Value[2]);
      }
    }
    else {
      isCorrect=false;
      ErrorMsg="Blad parsowania: Wykryto nieznany atrybut \""+string(sAttrName)+"\"!";
    }

    xercesc::XMLString::release(&sAttrName);
    xercesc::XMLString::release(&sAttrValue);

    if(!isCorrect) {
      delete pCuboid;
      throw invalid_argument(ErrorMsg);
    }
  }

  if(isName)
  {
    _rConfig._MobileObjects.push_back(pCuboid);
    cout << "Dodano obiekt: " << pCuboid->GetName() << endl;
  }
  else
  {
    delete pCuboid;
    throw runtime_error("Blad parsowania: Wykryto nienazwany obiekt!");
  }
}







/*!
 * Wykonuje operacje związane z wystąpieniem danego elementu XML.
 * W przypadku elementu \p "Action" będzie to utworzenie obiektu
 * reprezentującego odpowiednią działanie robota.
 * W przypadku elementu \p "Parameter" będzie to zapisanie parametrów
 * związanych z danym działaniem.
 * \param[in] rElemName - nazwa elementu XML.
 * \param[in] rAttrs - atrybuty napotkanego elementu XML.
 */
void XMLInterp4Config::WhenStartElement( const std::string           &rElemName,
		                         const xercesc::Attributes   &rAttrs
                                       )
{
  if (rElemName == "Lib") {
    ProcessLibAttrs(rAttrs);   return;   
  }

  if (rElemName == "Cube") {
    ProcessCubeAttrs(rAttrs);  return;
  }
}




/*!
 * Metoda jest wywoływana po napotkaniu nowego elementu XML, np.
 * gdy zostanie napotkany element:
   \verbatim
     <Action Name="Rotate">
   \endverbatim
 *  to poprzez parametr \e xscElemName jest dostęp do nazwy elementu
 *  tzn. \p Action, 
 *  \param[in] pURI - (Uniform Resource Identifier) jeżeli nie jest on wyspecyfikowany
 *                    poprzez użycie atrybutów \p xmlns (o ile jego użycie jest przewidziane w gramatyce,
 *                    w tym przykładzie nie jest to przewidziane), to będzie to napis pusty.
 *  \param[in] pLocalName -  umożliwia dostęp do nazwy elementu XML.
 *                 W podanym przykładzie nazwą elementu XML jest "Action".
 *  \param[in] pQName - pełna kwalifikowana nazwa. To ma znaczenie, gdy użyta jest przestrzeń nazwa.
 *                      Wówczas poprzez ten parametr można otrzymać nazwę elementu wraz z prefiksem
 *                      przestrzeni nazwa. Jeśli przestrzeń nazw nie jest użyta (taka jak w tym
 *                      przykładzie), to \p pQName i \p pLocalName dostaczają identyczne napisy.
 *  \param[in] rAttrs -  lista atrybutów danego symbolu XML.
 *                 W przykładzie pokazanym powyżej listę atrybutów
 *                 będą stanowiły napisy:
 */
/*
 * Te metode nalezy odpowiednio zdekomponowac!!!
 */
void XMLInterp4Config::startElement(  const   XMLCh* const            pURI,
                                      const   XMLCh* const            pLocalName,
                                      const   XMLCh* const            pQName,
				      const   xercesc::Attributes&    rAttrs
                                    )
{
  char* sElemName = xercesc::XMLString::transcode(pLocalName);
  cout << "+++ Poczatek elementu: "<< sElemName << endl;

  string strElemName = sElemName;
  xercesc::XMLString::release(&sElemName);

  WhenStartElement(strElemName, rAttrs);
}




/*!
 * Metoda zostaje wywołana w momencie zakończenia danego elementu
 * XML, np. jeżeli w pliku jest wpis:
   \verbatim
     <Lib Name="Rotate">
     </Lib>
   \endverbatim
 * to metoda ta zostanie wywołana po napotkaniu znacznika:
 * \verbatim</Lib>\endverbatim
 * Jeżeli element XML ma formę skróconą, tzn.
   \verbatim
     <Parametr Name="Rotate"/>
   \endverbatim
 * to wówczas wywołana metoda wywołana zostanie w momencie
 * napotkania sekwencji "/>"
 *  \param[in] pURI - (Uniform Resource Identifier) jeżeli nie jest on wyspecyfikowany
 *                    poprzez użycie atrybutów \p xmlns (o ile jego użycie jest przewidziane w gramatyce,
 *                    w tym przykładzie nie jest to przewidziane), to będzie to napis pusty.
 *  \param[in] pLocalName -  umożliwia dostęp do nazwy elementu XML.
 *                 W podanym przykładzie nazwą elementu XML jest "Lib".
 *  \param[in] pQName - pełna kwalifikowana nazwa. To ma znaczenie, gdy użyta jest przestrzeń nazwa.
 *                      Wówczas poprzez ten parametr można otrzymać nazwę elementu wraz z prefiksem
 *                      przestrzeni nazwa. Jeśli przestrzeń nazw nie jest użyta (taka jak w tym
 *                      przykładzie), to \p pQName i \p pLocalName dostaczają identyczne napisy.
 *                      
 */
void XMLInterp4Config::endElement(  const   XMLCh* const    pURI,
                                    const   XMLCh* const    pLocalName,
                                    const   XMLCh* const    pQName
                                 )
{
  char* sURI =  xercesc::XMLString::transcode(pURI);
  char* sElemName = xercesc::XMLString::transcode(pLocalName);
  char* sQName =  xercesc::XMLString::transcode(pQName);
  cout << "   URI: " << sURI << endl;
  cout << " QName: " << sQName << endl;
  cout << "----- Koniec elementu: "<< sElemName << endl;

  xercesc::XMLString::release(&sElemName);
  xercesc::XMLString::release(&sURI);
  xercesc::XMLString::release(&sQName); 
}




/*!
 * Metoda wywoływana jest, gdy napotkany zostanie błąd fatalny,
 * np.
  \verbatim
    <Action Name="Rotate">
    </Parametr>
  \endverbatim
 * \param[in] rException - zawiera informacje dotyczące błędu w dokumencie,
 *                         linii, kolumny itp.
 */
void XMLInterp4Config::fatalError(const xercesc::SAXParseException&  rException)
{
  char* sMessage = xercesc::XMLString::transcode(rException.getMessage());
  char* sSystemId = xercesc::XMLString::transcode(rException.getSystemId());

  cerr << "Blad fatalny! " << endl
      << "    Plik:  " << sSystemId << endl
      << "   Linia: " << rException.getLineNumber() << endl
      << " Kolumna: " << rException.getColumnNumber() << endl
      << " Informacja: " << sMessage 
      << endl;

  xercesc::XMLString::release(&sMessage);
  xercesc::XMLString::release(&sSystemId);
  throw runtime_error(">>> Blad fatalny parsowania pliku XML! <<<");
}

/*!
 * Metoda jest wywoływana, gdy napotkany zostanie błąd, który nie
 * jest traktowany jako fatalny. W ten sposób traktowane są błędy
 * występujące w opisie gramatyki dokumentu.
 * \param[in] rException - zawiera informacje dotyczące błędu. Informacje
 *                     te to opis błędu oraz numer linii, w której
 *                     wystąpił błąd.
 */
void XMLInterp4Config::error(const xercesc::SAXParseException&  rException)
{
  char* sMessage = xercesc::XMLString::transcode(rException.getMessage());
  char* sSystemId = xercesc::XMLString::transcode(rException.getSystemId());

  cerr << "Blad: " << endl
      << "    Plik:  " << sSystemId << endl
      << "   Linia: " << rException.getLineNumber() << endl
      << " Kolumna: " << rException.getColumnNumber() << endl
      << " Informacja: " << sMessage 
      << endl;

  xercesc::XMLString::release(&sMessage);
  xercesc::XMLString::release(&sSystemId);
  throw runtime_error(">>> Blad parsowania pliku XML! <<<");
}


/*!
 * Metoda wywołana jest w przypadku błędów, które nie są fatalne
 * i mogą być traktowane jako ostrzeżenia.
 * \param[in] rException - zawiera informacje dotyczące błędu w dokumencie,
 *                         linii, kolumny itp.
 */
void XMLInterp4Config::warning(const xercesc::SAXParseException&  rException)  
{
  char* sMessage = xercesc::XMLString::transcode(rException.getMessage());
  char* sSystemId = xercesc::XMLString::transcode(rException.getSystemId());

  cerr << "Ostrzezenie: " << endl
      << "    Plik:  " << sSystemId << endl
      << "   Linia: " << rException.getLineNumber() << endl
      << " Kolumna: " << rException.getColumnNumber() << endl
      << " Informacja: " << sMessage 
      << endl;

  xercesc::XMLString::release(&sMessage);
  xercesc::XMLString::release(&sSystemId);
}

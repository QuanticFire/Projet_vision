#pragma once

/*
Adaptation du projet seuillage auto aux besoins du projet OuiOui
*/

#include "ImageClasse.h"
#include "ImageNdg.h"
#include "ImageCouleur.h"
#include "ImageDouble.h"

#include <windows.h>

class ClibTraitement {

	///////////////////////////////////////
private:
	///////////////////////////////////////

	// data n�cessaires � l'IHM donc fonction de l'application cibl�e
	int						nbDataImg; // nb champs Texte de l'IHM
	std::vector<double>		dataFromImg; // champs Texte de l'IHM
	CImageCouleur*          imgPt;       // 

	///////////////////////////////////////
public:
	///////////////////////////////////////

	// constructeurs
	_declspec(dllexport) ClibTraitement(); // par d�faut

	_declspec(dllexport) ClibTraitement(int nbChamps, byte* data, int stride, int nbLig, int nbCol); // par image format bmp C#

	_declspec(dllexport) ClibTraitement(byte * datain, int stride, int nbLig, int nbCol, int seuilB, int seuilH); // cr�ation d'une image rogn�e

	_declspec(dllexport) ~ClibTraitement();

	// get et set 

	_declspec(dllexport) int lireNbChamps() const {
		return nbDataImg;
	}

	_declspec(dllexport) double lireChamp(int i) const {
		return dataFromImg.at(i);
	}

	_declspec(dllexport) CImageCouleur* imgData() const {
		return imgPt;
	}

	//m�thodes suppl�mentaires
	_declspec(dllexport) void Traitement2(int nbChamps, byte* data, int stride, int nbLig, int nbCol, double parametres[10], int nbChamps_p, byte * data_p, int stride_p, int nbLig_p, int nbCol_p); //nouvelle m�thode de traitement test
	_declspec(dllexport) void TraitementMatching(byte * data, int stride, int nbLig, int nbCol, byte * data_p, int stride_p, int nbLig_p, int nbCol_p);
	_declspec(dllexport) void ClibTraitement::TraitementRognage(int nbChamps, byte * data, int stride, int nbLig, int nbCol, double parametres[10], int nbChamps_p, byte * data_p, int stride_p, int nbLig_p, int nbCol_p);
	_declspec(dllexport) void copydata(byte* data, int stride);
};

/****************************************************************************************************************

DEBUT DES METHODES UTILISEES EN C#
Rennomage des m�thodes et constructeurs de la classe C++ pour contr�ler ce qu'on envoie en C#

****************************************************************************************************************/

// Convention C pour que le constructeur C# puisse les voir selon leur calling convention
extern "C" _declspec(dllexport) ClibTraitement* objetLib()
{
	ClibTraitement* pImg = new ClibTraitement();
	return pImg;
}

// m�thode de traitement par d�faut: constructeur
extern "C" _declspec(dllexport) ClibTraitement* objetLibDataImg(int nbChamps, byte* data, int stride, int nbLig, int nbCol)
{
	ClibTraitement* pImg = new ClibTraitement(nbChamps,data,stride,nbLig,nbCol);
	return pImg;
}

// constructeur de d'image rogn�e
extern "C" _declspec(dllexport) ClibTraitement* Rognage(byte* datain, int stride, int nbLig, int nbCol, int seuilB, int seuilH) {
	ClibTraitement* pImg = new ClibTraitement(datain, stride, nbLig, nbCol, seuilB, seuilH);
	return pImg;
}

// m�thode de traitement suppl�mentaire
extern "C" _declspec(dllexport) ClibTraitement* traitementTest(int nbChamps, byte* data, int stride, int nbLig, int nbCol, double parametres[10], int nbChamps_p, byte * data_p, int stride_p, int nbLig_p, int nbCol_p)
{
	ClibTraitement* pImg = new ClibTraitement();
	pImg->Traitement2(nbChamps, data, stride, nbLig, nbCol, parametres, nbChamps_p, data_p, stride_p, nbLig_p, nbCol_p);
	return pImg;
}

extern "C" _declspec(dllexport) ClibTraitement* traitementRogne(int nbChamps, byte* data, int stride, int nbLig, int nbCol, double parametres[10], int nbChamps_p, byte * data_p, int stride_p, int nbLig_p, int nbCol_p)
{
	ClibTraitement* pImg = new ClibTraitement();
	pImg->TraitementRognage(nbChamps, data, stride, nbLig, nbCol, parametres, nbChamps_p, data_p, stride_p, nbLig_p, nbCol_p);
	return pImg;
}

extern "C" _declspec(dllexport) ClibTraitement* PatternMatching(byte* data, int stride, int nbLig, int nbCol, byte * data_p, int stride_p, int nbLig_p, int nbCol_p)
{
	ClibTraitement* pImg = new ClibTraitement();
	pImg->TraitementMatching(data, stride, nbLig, nbCol, data_p, stride_p, nbLig_p, nbCol_p);
	return pImg;
}

// attention : n�cessaire de savoir o� trouver le bon param�tre (possiblilit� d'enum en C# taduits en nombres en C.
extern "C" _declspec(dllexport) double valeurChamp(ClibTraitement* pImg, int i)
{
	return pImg->lireChamp(i);
}

// cherche les donn�es de l'image dans ClImage
extern "C" _declspec(dllexport) void dataFromImg(ClibTraitement* pImg, byte* scan0, int stride) {
	pImg->copydata(scan0, stride);
}

//cherche la largeur et la hauteur

extern "C" _declspec(dllexport) int imgHauteur(ClibTraitement* pImg) {
	return pImg->imgData()->lireHauteur();
}

extern "C" _declspec(dllexport) int imgLargeur(ClibTraitement* pImg) {
	return pImg->imgData()->lireLargeur();
}


/****************************************************************************************************************

FIN DES METHODES UTILISEES EN C#

****************************************************************************************************************/

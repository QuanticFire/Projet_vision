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

	// data nécessaires à l'IHM donc fonction de l'application ciblée
	int						nbDataImg; // nb champs Texte de l'IHM
	std::vector<double>		dataFromImg; // champs Texte de l'IHM
	CImageCouleur*          imgPt;       // 

	///////////////////////////////////////
public:
	///////////////////////////////////////

	// constructeurs
	_declspec(dllexport) ClibTraitement(); // par défaut

	_declspec(dllexport) ClibTraitement(int nbChamps, byte* data, int stride, int nbLig, int nbCol); // par image format bmp C#

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

	//méthodes supplémentaires
	_declspec(dllexport) void Traitement2(int nbChamps, byte* data, int stride, int nbLig, int nbCol, double parametres[10], int nbChamps_p, byte * data_p, int stride_p, int nbLig_p, int nbCol_p); //nouvelle méthode de traitement test
	_declspec(dllexport) void TraitementMatching(byte * data, int stride, int nbLig, int nbCol, byte * data_p, int stride_p, int nbLig_p, int nbCol_p);
	_declspec(dllexport) void ClibTraitement::TraitementRognage(int nbChamps, byte * data, int stride, int nbLig, int nbCol, double parametres[10], int nbChamps_p, byte * data_p, int stride_p, int nbLig_p, int nbCol_p);
	_declspec(dllexport) int* props(byte* data);
};

/****************************************************************************************************************

DEBUT DES METHODES UTILISEES EN C#
Rennomage des méthodes et constructeurs de la classe C++ pour contrôler ce qu'on envoie en C#

****************************************************************************************************************/

// Convention C pour que le constructeur C# puisse les voir selon leur calling convention
extern "C" _declspec(dllexport) ClibTraitement* objetLib()
{
	ClibTraitement* pImg = new ClibTraitement();
	return pImg;
}

// méthode de traitement par défaut: constructeur
extern "C" _declspec(dllexport) ClibTraitement* objetLibDataImg(int nbChamps, byte* data, int stride, int nbLig, int nbCol)
{
	ClibTraitement* pImg = new ClibTraitement(nbChamps,data,stride,nbLig,nbCol);
	return pImg;
}

// méthode de traitement supplémentaire
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

// attention : nécessaire de savoir où trouver le bon paramètre (possiblilité d'enum en C# taduits en nombres en C.
extern "C" _declspec(dllexport) double valeurChamp(ClibTraitement* pImg, int i)
{
	return pImg->lireChamp(i);
}

// cherche les propriétés de l'image c'est un pointeur à supprimer en c#
extern "C" _declspec(dllexport) int* proprietes(ClibTraitement* pImg, byte* data) {
	return pImg->props(data);
}

/****************************************************************************************************************

FIN DES METHODES UTILISEES EN C#

****************************************************************************************************************/

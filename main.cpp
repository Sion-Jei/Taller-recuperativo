#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "omp.h"
#include <algorithm>
#include <list>
#include <string>
#include <map>
#include <numeric>
#include <math.h>

std::map<double, double> smi;
std::map <std::string, double> dollars;
std::map<double, double> cash;
std::list<double> buff;

auto convertYear(std::map<double, double> y, std::map<std::string, double> d) {
    std::map<double, double> cash;
    auto years = y.begin(); // Inicializa variable e iteracion       
    while (years != y.end()) { // Mientras year no haya terminado sigue ejecutandose
        buff = {}; // Reincia lista por cada año
        auto dollars = d.begin();
        while (dollars != d.end()) {            
            if (years->first == std::stod(dollars->first.substr(0, 4))) { //Comparacion entre año del SMI y los valores diarios del dollar                
                buff.push_back(dollars->second); // Agrega los valores de dollar
            }
            ++dollars;
        }
        double sum = std::accumulate(std::begin(buff), std::end(buff), 0.0); // Suma todos los valores de la lista de los valores de dollar
        double result = sum / buff.size(); //promedio
        cash.insert(std::pair<double, double>(years->first, years->second/result)); //salario minimo convertido a dollar
        ++years;
    }
    return cash;
}
auto linearRegression(std::map<double, double> m) {
    double n = m.size(); // m (mapa)
    double sx = 0;
    double sxx = 0;
    double sy = 0;
    double syy = 0;
    double sxy = 0;
    auto iter = m.begin(); // Inicializando la iteracion del mapa
    while (iter != m.end()) {
        sx += iter->first; //sumatoria años salario minimo
        sxx += pow(iter->first, 2); // x**2 - SUMA
        sy += iter->second; //sumatoria salario minimo en dollar
        syy += pow(iter->second, 2); // y**2 - SUMA
        sxy += iter->first * iter->second; // Multiplica año por valor corresp del dollar
        ++iter;
    }
    double b = (n * sxy - sx * sy) / (n * sxx - sx * sx); // Pendiente
    double a = (1 / n) * sy - b * (1 / n) * sx; // Coeficiente
    std::cout << "y = " << b << "x " << a;
}

auto csv_smi(std::string path) {
    std::map<double, double> map_tmp;
    std::string file_path = path;
    std::ifstream csv;
    csv.open(file_path);
    std::string line;
    getline(csv, line);
    while (csv.peek() != EOF) {        
        getline(csv, line);
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
        size_t position = line.find(";") + 1;
        std::string key_k = line.substr(0, line.find(";"));
        std::string key_v = line.substr(position);
        key_k.erase(std::remove(key_k.begin(), key_k.end(), '\"'), key_k.end());
        key_v.erase(std::remove(key_v.begin(), key_v.end(), '\"'), key_v.end());
        double key_kd = std::stod(key_k);
        double key_vd = std::stod(key_v);
        map_tmp.insert(std::pair<double, double>(key_kd, key_vd));
    }
    csv.close();
    return map_tmp;
}

auto csv_dollars(std::string path) {
    std::map<std::string, double> map_tmp;
    std::string file_path = path;
    std::ifstream csv;
    csv.open(file_path);
    std::string line;
    getline(csv, line); // Omite la primera linea de descripcion
    while (csv.peek() != EOF) { //Revisa si ha terminado de leer el archivo
        getline(csv, line); // Guarda linea del archivo en variable line
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end()); // Elimina los saltos de linea
        size_t position = line.find(";") + 1; // Guarda las posiciones de ;
        std::string key_k = line.substr(0, line.find(";")); // Variable toma lo que este antes de ;
        std::string key_v = line.substr(position); // Variable toma todo lo que este despues de las posiciones con ;
        key_k.erase(std::remove(key_k.begin(), key_k.end(), '\"'), key_k.end()); // Elimina comillas
        key_v.erase(std::remove(key_v.begin(), key_v.end(), '\"'), key_v.end());
        double key_vd = std::stod(key_v); // Convierte string valor dollar en doble
        map_tmp.insert(std::pair<std::string, double>(key_k, key_vd));
    }
    csv.close();
    return map_tmp;    
}
int main() {

    smi = csv_smi("smi.csv");
    dollars = csv_dollars("dollars.csv");
    cash = convertYear(smi, dollars);
    linearRegression(cash);

    std::cout << std::endl;
    return 0;
}
#pragma once // Evita que el archivo se incluya múltiples veces en la compilación
 

class Position {
    private:
        int row;
        int col;
    
    public:
        // Constructores
        Position(); // Contructor por defecto
        Position(int r, int c); // Constructor parametrizado 


        // Getters (Necesarios para acceder a row y col desde otras clases
        int getRow( )const;
        int getCol( )const;

        // Metodo definido en el diagrama UML 
        // Se pasa por referencia constante (const Position&) por rendimiento y seguridad 
        bool equals (const Position& p) const;

        // Sobrecarga del operador ==
        // Hace qie en el futuro puedas hyacer: si (pos1 == pos2)
        bool operator == (const Position& p) const;

};


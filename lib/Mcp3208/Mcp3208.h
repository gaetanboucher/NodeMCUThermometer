
// default SPI pin assignment
#define SELPIN    D8 //Selection Pin 
#define DATAOUT   D7 //MOSI 
#define DATAIN    D6 //MISO 
#define SPICLOCK  D5 //Clock

class Mcp3208 
{
public:
    Mcp3208();
    void setCSpin(int csPin);
    void setDataOutpin(int dataOutPin);
    void setDataInpin(int dataInPin);
    void setClock(int clock);
    int read(int channel);
};
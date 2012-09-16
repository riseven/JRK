#ifndef __R_PROTOCOLO_H__
#define __R_PROTOCOLO_H__

namespace Red
{

class Protocolo
{
public:
    class Unidad
    {
    public:
        enum EnumUnidad
        {
            Soldado = 1,
            CuartelGeneral ,
            PozoDeExtraccion ,
            CentroDeComunicaciones ,
            Barracones ,
            PlantaDeVehiculos ,
            Laboratorio ,
            AcademiaDeParacaidismo ,
            Hangar ,
            SiloDeMisiles ,
            AcademiaDeFrancotiradores ,
            TorreDeVigilancia ,
            Ingeniero ,
            Artificiero ,
            Medico ,
            Zapador ,
            Jeep ,
            Tanque ,
            ArtilleriaMovil ,
            SAMMovil ,
            TransporteBlindado ,
            Helicoptero ,
            Paracaidista ,
            Francotirador ,
            
            Petroleo,
        };
    };
    
    class Cliente
    {
    public:
        enum EnumComando
        {
            UnirsePartida = 1,
            MensajeChatMenu,
            CambiarColor,
            MensajeChatJuego,
            MoverUnidad,
            DetenerUnidad,
            CambiarModoUnidad,
            AtacarZona,
            AtacarUnidad,
            // Cuartel general
            ConstruirIngeniero,
            DesarrollarMotivacion,
            DesarrollarMotivacionMasDistancia,
            // Barracones
            ConstruirSoldado,
            DesarrollarArmaduraSoldado,
            DesarrollarVelocidadDisparoSoldado,
            // Ingeniero
            ConstruirEdificio,
            RepararIngeniero,
            // Soldado
            CorrerSoldado,
        };
    };
    
    class Servidor
    {
    public:
        enum EnumServidor
        {
            UnirsePartida = 1,
            AbandonoJugador,
            MensajeChatMenu,
            SetColorJugador,
            SetJugadorPropio,
            SetMapa,
            EmpezarPartida,
            CrearUnidad,
            SetPosicionUnidad,
            SetAnguloUnidad,
            SetPorcentajeConstruidoUnidad,
            SetEstadoTecnologia,
            MensajeChatJuego,
            SetDinero,
            SetOcupadoPetroleo,
            SetDia,
            SetInvierno,
        };
    };
    
    class Tecnologia
    {
    public:
        enum EnumTecnologia
        {
            Motivacion = 0,
            MotivacionMasDistancia,
            ArmaduraSoldado,
            VelocidadDisparoSoldado,
        };
        
        static const int NumTecnologias = 4 ;
    };
    
    class EstadoTecnologia
    {
    public:
        enum EnumEstadoTecnologia
        {
            NoDesarrollado = 0,
            EnDesarrollo,
            Desarrollado
        };
    };
    
    class ModoUnidad
    {
    public:
        enum EnumModoUnidad
        {
            Agresivo = 0,
            Defensivo ,
            MantenerPosicion ,
            Pasivo
        };
    };
    class Proyectiles
    {
    public:
           enum EnumProyectiles
           {
                Bala=0 //Proyectil de metralleta
           };
    };
    
    class Armas
    {
    public:
           enum EnumArmas
           {
                Metralleta=0
           };
    }; 
};

}

#endif

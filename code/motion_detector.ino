//=========================================
// Détecteur de mouvement
//=========================================
#define BTN_PIN   2
#define LED_V     8
#define LED_J     9
#define LED_R     10
#define BUZZER    11

int systemeActif = 0;
int alarme = 0;
unsigned long tempsActivation = 0;
unsigned long debutAlarme = 0;
void setup()
{
    pinMode(BTN_PIN, INPUT_PULLUP);
    pinMode(LED_V, OUTPUT);
    pinMode(LED_J, OUTPUT);
    pinMode(LED_R, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    Serial.begin(9600);
    digitalWrite(LED_V, HIGH);
    digitalWrite(LED_J, LOW);
    digitalWrite(LED_R, LOW);
}

void loop()
{
    //==========================
    // Gestion du bouton
    if (digitalRead(BTN_PIN) == LOW)
    {
        delay(250);
        while (digitalRead(BTN_PIN) == LOW);
        if (systemeActif == 0)
        {
            // Activation
            systemeActif = 1;
            alarme = 0;
            tempsActivation = millis();
            digitalWrite(LED_V, LOW);
            digitalWrite(LED_J, HIGH);
            Serial.println("SYSTEME ACTIVE");
        }
        else
        {
            // Désactivation
            systemeActif = 0;
            alarme = 0;
            digitalWrite(LED_V, HIGH);
            digitalWrite(LED_J, LOW);
            digitalWrite(LED_R, LOW);

            noTone(BUZZER);

            Serial.println("SYSTEME ARRETE");
        }
    }

    //==========================
    // Déclenchement après 10 s
    //==========================

    if (systemeActif == 1 && alarme == 0)
    {
        if (millis() - tempsActivation >= 10000)
        {
            alarme = 1;
            debutAlarme = millis();
            Serial.println("ALARME");
        }
    }

    //==========================
    // Alarme pendant 5 secondes
    //==========================

    if (alarme == 1)
    {
        if (millis() - debutAlarme < 5000)
        {
            if ((millis() / 250) % 2 == 0)
            {
                digitalWrite(LED_R, HIGH);
                tone(BUZZER, 1000);
            }
            else
            {
                digitalWrite(LED_R, LOW);
                noTone(BUZZER);
            }
        }
        else
        {
            alarme = 0;

            digitalWrite(LED_R, LOW);
            noTone(BUZZER);

            // Empêche un redémarrage immédiat
            tempsActivation = millis();
        }
    }
}

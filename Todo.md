

## **System-Architektur (Top-Level)**

**1. Stat Layer**
Numerische Werte, fortlaufend verändert.
**2. Body Layer**
Körperteile und physische Zustände.
**3. Status Effect Layer**
Buff/Debuff-System, zeitlich oder zustandsbasiert.
**4. Environment Layer**
Wetter, Temperatur, Wind, Nässe, Strahlung.
**5. Progression Layer**
Level, XP, Skills, Attribute.

Alles funktioniert **modular**: jedes Layer kann **einzeln aktiviert/deaktiviert** werden.

---

## **Stat Layer – Werte & Dynamik**

**Grundwerte (skalierbar):**

* Health_Core (Lebensfähigkeit)
* Stamina (Kurzzeit-Energie)
* Energy (Langzeit-Erschöpfung)
* Hunger
* Thirst
* Fatigue (Schlafbedarf)
* BodyTemperature
* Wetness
* BloodLevel (für Blutverlust-System)
* Sanity (für Horror/Isolation)
* Infection_Level (für Zombie / Krankheit / Parasiten)
* Toxicity (Gift / Chemikalien / schlechte Nahrung)
* Strength, Dexterity, Intelligence, Endurance (optional RPG)

**Meta-Mechaniken:**

* **Max-Werte verändern sich durch Body Condition, Umgebung, Effekte**
* **Regeneration/Decay-Regeln sind parametrierbar** per Kurven/Datentabellen

---

## **Body Layer – Körpermodell**

**Körperteile (Konfigurierbar):**

* Head
* Torso
* LeftArm / RightArm
* LeftLeg / RightLeg

**Zustände je Körperteil:**

* Condition (0–100)
* Fracture (Ja/Nein)
* BleedingRate
* BurnLevel (0–3)
* InfectionRate
* PainLevel

**Auswirkungen:**

* Bein verletzt → Laufgeschwindigkeit, Ausdauerverbrauch
* Arm verletzt → Zielgenauigkeit / Waffen-Sway
* Torso verletzt → MaxHealth & maximale Stamina
* Kopf → Sanity / Ohnmachtschance

---

## **Status Effect Layer – Buff/Debuff Framework**

Effekte sind **Objekte** mit Regeln, keine Einzelfälle hartverdrahtet.

**Effekt-Typen:**

* Kurzfristig (StaminaBoost, AdrenalineRush)
* Zustandsgesteuert (Cold, Heatstroke, Hypothermia, Starvation)
* Dauerhaft (Perks, Traits, Mutations)
* Stapelbar (z. B. Gift-Stacks)
* Vorbereitbar (z. B. “Infektion wächst über Zeit”)

**Effekt-Parameter:**

* Trigger (Schaden, Zeit, Umgebung, Körperteil)
* Dauer (fest oder bedingt)
* Priorität (welches überschreibt wen)
* UI-Tag (Anzeige-Icon)

**Beispiele:**

* **Exhausted** → StaminaRegeneration stark reduziert
* **Panicked** → Sanity drop, Zielgenauigkeit sinkt
* **AdrenalineRush** → Schmerz ignoriert, Ausdauer +20%, läuft aus
* **InfectionStage2** → Erhöhte Temperatur, Sprachzittern, Tremor

---

## **Environment Layer – Weltintegration**

**Eingänge:**

* AmbientTemperature
* WindIntensity
* RainLevel
* SnowLevel
* ShelterState (Innen / Außen)
* Clothing_Insulation (Rüstung / Kleidung)
* RadiationFactor (optional)

**Berechnungslogik:**
BodyTemperature bewegt sich immer Richtung AmbientTemperature,
moduliert von:

```
Exposure = (1 - Clothing_Insulation) + Wetness*0.02 + WindIntensity*0.1
```

Wetness trocknet abhängig von:

* Körpertemperatur
* Wind
* Unterstand

---

## **Progression Layer – Level & Skills**

* XP kann von **Belohnungssystem**, **Nutzungssystem** oder **Überleben über Zeit** kommen.
* Level steigert **AttributePoints** und **SkillPoints**.
* Fähigkeiten können Stats oder Effekte dauerhaft verändern.

Beispiele:

* “Resilient”: reduziert Fatigue-Buildup
* “Iron Lungs”: Stamina-Regeneration +10%
* “Pathfinder”: Weniger Energieverbrauch beim Gehen

---

## **Funktionale Erweiterungen (um es *wirklich* flexibel zu machen)**

**Globale Funktionen:**

* `ApplyStatChange(Source, StatName, Amount, ReasonTag)`
* `ApplyStatusEffect(EffectID, Stacks)`
* `RemoveEffect(EffectID)`
* `FractureLimb(LimbID)`
* `SetWetness(Value)`
* `ApplyInfection(Severity)`
* `HealLimb(LimbID, Method)`

**Abfragefunktionen:**

* `IsCritical()`
* `IsHypothermic()`
* `IsDehydrated()`
* `IsStarving()`
* `IsInShock()`

**Balancing-Haken:**

* Stat-Werte werden **von Gameplay-Items modifiziert**:

  * Nahrung, Vitamine, Drogen, Schmerzmittel, Verbände, Kleidung, Rüstung

**Gameplay-Event-Auslöser:**

* OnSevereCold
* OnBloodLoss
* OnBrokenBone
* OnInfectionProgress
* OnBlackoutRisk
* OnAdrenalineTriggered





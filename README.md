# Compilerbauwerkzeuge

Das ist die Lösung der Abgabe für die Vorlesung "Compilerbauwerkzeuge" an der DHBW Stuttgart. Aufgrund von einem Missgeschick wurde die Aufgabe für unser Team verändert, wodurch der Umfang ein wenig höher ist als bei der normalen Abgabe. Im Anhang sind die einzelnen Aufgabe aufgelistet und jeweils welcher Ordner die Aufgabe löst. Kleiner Hinweis nebenbei. Unsere Lösung ist nicht die effektivste welche möglich ist. Besser gesagt eher sehr uneffektiv.

#1. Teilaufgabe:
Schreiben Sie ein Flex-File fur einen Scanner, der aus einer Datei eine ¨
Formel im beschriebenen Format einliest und die einzelnen Token
zuruckgibt. ¨
	1) Ausgabe mit printf() zur Kontrolle
	2) Ruckgabe mit ¨ return() fur Parser
	
Ordner: projectPLscanner

#2. Teilaufgabe:
Schreiben Sie ein Bison-File fur einen Parser, der mit Hilfe Ihres Scanners ¨
eine Formel einliest und auf korrekte Syntax testet.
Der Parser sollte zur Kontrolle die gelesenen Token und die angewendete
Regel ausgeben.

Ordner: projectPLparser

#3. Teilaufgabe:
Entwickeln Sie C-Datenstrukturen (structs) fur die genannten Ausdrücke.
Hilfreich sind Funktionen für die folgenden Aufgaben: ¨
Erzeugen eines neuen Ausdrucks, z.B.
term* createTerm (char* varfunc, termList* tl),
Einfugen und Entfernen von Listen-Elementen, ¨
Textuelle Ausgabe von Datentypen zur Kontrolle.

Ordner: projectCStructs

#4. Teilaufgabe:
Modifizieren Sie Ihren Parser so, dass er nicht nur die Syntax testet,
sondern auch die Formel in der Eingabe-Datei in die von Ihnen definierten
Datenstruktur einliest.
Ergebnis sollte eine globale Variable (z.B. formula* f) sein, die die
Formel der Eingabedatei enthält.

Ordner: projectFormula

#5. Teilaufgabe:
Schreiben Sie eine Funktion
formula* nnf(formula* input)
die die eingelesene Formel entsprechend den genannten Regeln in die
Negations-Normalform konvertiert.
Verwenden Sie anschliessend die von Ihnen geschriebene
Ausgabe-Funktion, um die normalisierte Formel auszugeben.

Ordner: projectNNF

#6. Teilaufgabe - Umbennen von Variablen:
Um zu verhindern, dass dieselbe Variable mehrmals hinter einem Quantor vorkommt 
(was z.B. durch das Entfernen der Aequivalenzen passiert), werden die Variablen 
systematisch umbenannt. Am einfachsten ist dies dadurch zu erreichen, dass man die Formel 
einmal komplett durchlaeuft und die erste quantifizierte Variable in v1 umbenennt, die zweite in 
v2 usw. Hierbei ist natuerlich zu beachten, dass man die Variable nur im Gueltigkeitsbereich des 
Quantors (in der Baumdarstellung der Formel: Unterhalb des Quantors) umbenennt.

Z.B.: all x (P(x) & Q(x)) & all x S(x,x) wird zu
all v1 (P(v1) & Q(v1)) & all v2 S(v2,v2)

Ordner: projectRename

#7. Teilaufgabe - Existentiell quantifizierte Variablen werden durch Terme ersetzt (Skolemisierung):
Hierzu wird die Formel in ihrer Baumstruktur von oben nach unten durchlaufen, und eine existentiell 
quantifizierte Variable wird durch ein neues Funktionssymbol ersetzt, dessen Stelligkeit von der Anzahl 
der Allquantoren abhaengt, die im Baum oberhalb des Existenzquantors stehen. Gibt es keine solichen 
Allquantoren, hat man ein nullstelliges Funktionssymbol, also ein Konstantensymbol.

Fuer die neuen Funktionssymbole koennen Sie einfach f1, f2, ... verwenden.

Z.B.: ex v1 P(v1) & all v2 ex v3 S(v2,v3) wird zu
P(f1) & all v2 S(v2,f2(v2))

Ordner: projectSkolem

#8. Teilaufgabe - Entfernung der Allquantoren:
Diese koennen einfach weggelassen werden.

Z.B.: all v2 S(v2,f2(v2)) wird zu
S(v2,f2(v2))

Ordner: projectRenameAll

#9. Teilaufgabe - Distributivgesetz:
Anwendung des Distributivgesetzes: Falls eine Konjunktion innerhalb einer Disjunktion vorkommt,
z.B. (A & B) | C, ersetzen Sie diese durch (A | C) & (B | C). Hierbei ist zu beachten, dass die Formel 
"C" selbst wieder eine Konjunktion sein kann, so dass hier weitere Schritte notwendig werden.

Ordner: projectDistributive

#10. Teilaufgabe:
"Einebnen" (flatten) von hierarchischen Konjunktionen und Disjunktionen, z.B. (A | (B | C)) zu
(A | B | C). Dies ist auf den ersten Blick sehr einfach, benoetigt aber andere Datenstrukturen, 
denn eine Konjunktion / Disjunktion hat jetzt nicht mehr genau zwei Argumente, sondern beliebig viele. 
Deshalb benoetigen Sie hier wieder eine verkettete Liste fuer die Subformeln. An diese Datenstrukturen 
muessen dann auch die Parser-Regeln fuer Konjunktion und Disjunktion angepasst werden.

Am Ende erhalten Sie eine Formel, die eine Konjunktion von Disjunktionen von Literalen ist, z.B. 
(A | B) & (C | D | E) & (F | G | H | I).

(Hierbei sind A-I Literale, also potentiell negierte Atome).

Ordner: projectKNF-2


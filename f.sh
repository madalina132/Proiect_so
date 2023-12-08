# Verifica daca scriptul primeste doua argumente
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <character>"
    exit 1
fi

# Extrage caracterul primit ca argument
character="$1"

# Initializare contor
counter_l=0
counter_ch=0

# Citeste linie cu linie de la intrarea standard
while IFS= read -r line; do
    if [ -z "$line" ]; then
        break  # Iesire din bucla în cazul unei linii goale
    fi

    # Verifica daca linia respecta conditiile
    if [[ "$line" =~ ^[A-Z][a-z0-9]*[a-z][0-9]*\.$ ]]; then
        ((counter_l++))
    fi

    # Utilizeaza grep pentru a numara aparitiile caracterului in linie
    occurrences=$(echo "$line" | grep -o "$character" | wc -l)
    ((counter_ch += occurrences))
done

# Afișează rezultatul
echo "Numărul de linii care respectă condițiile: $counter_l"
echo "Numărul de apariții ale caracterului '$character': $counter_ch"

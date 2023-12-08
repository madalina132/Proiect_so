# Verifica daca scriptul primeste doua argumente
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <character>"
    exit 1
fi

# Extrage caracterul primit ca argument
character="$1"

# Initializare contor
counter_lines=0
counter_character=0

# Citeste linie cu linie de la intrarea standard
while IFS= read -r line; do
    if [ -z "$line" ]; then
        break  # Iesire din bucla în cazul unei linii goale
    fi

    # Verifica daca linia respecta conditiile
    if [[ "$line" =~ ^[A-Z][a-z0-9]*[a-z][0-9]*\.$ ]]; then
        ((counter_lines++))
    fi

    # Utilizeaza grep pentru a numara aparitiile caracterului in linie
    occurrences=$(echo "$line" | grep -o "$character" | wc -l)
    ((counter_character += occurrences))
done

# Afișează rezultatul
echo "Numărul de linii care respectă condițiile: $counter_lines"
echo "Numărul de apariții ale caracterului '$character': $counter_character"

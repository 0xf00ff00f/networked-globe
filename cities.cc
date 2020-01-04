#include "cities.h"

// source: https://www.infoplease.com/world/world-geography/major-cities-latitude-longitude-and-corresponding-time-zones

const std::vector<city> &get_cities()
{
    static const std::vector<city> cities = {
        {"Aberdeen", "Scotland", 0.99746, -0.03752},
        {"Adelaide", "Australia", -0.60941, 2.41903},
        {"Algiers", "Algeria", 0.64286, 0.05236},
        {"Amsterdam", "Netherlands", 0.91397, 0.08523},
        {"Ankara", "Turkey", 0.69668, 0.57450},
        {"Asunción", "Paraguay", -0.44070, -1.00647},
        {"Athens", "Greece", 0.66264, 0.41393},
        {"Auckland", "New Zealand", -0.64344, 3.04996},
        {"Bangkok", "Thailand", 0.23998, 1.75406},
        {"Barcelona", "Spain", 0.72228, 0.03752},
        {"Beijing", "China", 0.69668, 2.03185},
        {"Belém", "Brazil", -0.02560, -0.84619},
        {"Belfast", "Northern Ireland", 0.95324, -0.10356},
        {"Belgrade", "Serbia", 0.78307, 0.35837},
        {"Berlin", "Germany", 0.91630, 0.23417},
        {"Birmingham", "England", 0.91484, -0.03345},
        {"Bogotá", "Colombia", 0.07912, -1.29591},
        {"Bombay", "India", 0.33161, 1.27060},
        // {"Bordeaux", "France", 0.78249, -0.00902},
        // {"Bremen", "Germany", 0.92648, 0.15388},
        {"Brisbane", "Australia", -0.47967, 2.67268},
        {"Bristol", "England", 0.89826, -0.04509},
        {"Brussels", "Belgium", 0.88779, 0.07621},
        {"Bucharest", "Romania", 0.77522, 0.45582},
        {"Budapest", "Hungary", 0.82903, 0.33307},
        {"Buenos Aires", "Argentina", -0.60359, -1.01869},
        {"Cairo", "Egypt", 0.52418, 0.54716},
        {"Calcutta", "India", 0.39386, 1.54287},
        {"Canton", "China", 0.40346, 1.97659},
        {"Cape Town", "South Africa", -0.59196, 0.32056},
        // {"Caracas", "Venezuela", 0.18268, -1.16995},
        {"Cayenne", "French Guiana", 0.08407, -0.91281},
        {"Chihuahua", "Mexico", 0.49946, -1.85150},
        {"Chongqing", "China", 0.51953, 1.85994},
        {"Copenhagen", "Denmark", 0.97157, 0.21933},
        {"Córdoba", "Argentina", -0.54920, -1.11992},
        {"Dakar", "Senegal", 0.25598, -0.30485},
        {"Darwin", "Australia", -0.21758, 2.28376},
        {"Djibouti", "Djibouti", 0.20071, 0.75136},
        {"Dublin", "Ireland", 0.93084, -0.10908},
        {"Durban", "South Africa", -0.52156, 0.53902},
        {"Edinburgh", "Scotland", 0.97593, -0.05527},
        // {"Frankfurt", "Germany", 0.87470, 0.15155},
        {"Georgetown", "Guyana", 0.11781, -1.01665},
        {"Glasgow", "Scotland", 0.97448, -0.07418},
        {"Guatemala City", "Guatemala", 0.25511, -1.57981},
        {"Guayaquil", "Ecuador", -0.03782, -1.39510},
        // {"Hamburg", "Germany", 0.93462, 0.17511},
        {"Hammerfest", "Norway", 1.23278, 0.41248},
        // {"Havana", "Cuba", 0.40375, -1.43786},
        {"Helsinki", "Finland", 1.05011, 0.43633},
        {"Hobart", "Tasmania", -0.74816, 2.57116},
        {"Hong Kong", "China", 0.38979, 1.99288},
        {"Iquique", "Chile", -0.35197, -1.22377},
        {"Irkutsk", "Russia", 0.91630, 1.82096},
        {"Jakarta", "Indonesia", -0.10937, 1.86401},
        {"Johannesburg", "South Africa", -0.45728, 0.48986},
        {"Kingston", "Jamaica", 0.31387, -1.34070},
        {"Kinshasa", "Congo", -0.07505, 0.26674},
        {"Kuala Lumpur", "Malaysia", 0.05469, 1.77500},
        {"La Paz", "Bolivia", -0.28711, -1.19322},
        {"Leeds", "England", 0.93811, -0.02618},
        {"Lima", "Peru", -0.20944, -1.34449},
        {"Lisbon", "Portugal", 0.67602, -0.15970},
        {"Liverpool", "England", 0.93230, -0.05236},
        {"London", "England", 0.89943, -0.00145},
        // {"Lyons", "France", 0.79849, 0.08436},
        {"Madrid", "Spain", 0.70569, -0.06458},
        {"Manchester", "England", 0.93375, -0.03927},
        {"Manila", "Philippines", 0.25453, 2.11098},
        // {"Marseilles", "France", 0.75631, 0.09308},
        {"Mazatlán", "Mexico", 0.40492, -1.85732},
        {"Mecca", "Saudi Arabia", 0.37495, 0.69377},
        {"Melbourne", "Australia", -0.65944, 2.53015},
        {"Mexico City", "Mexico", 0.33918, -1.72991},
        // {"Milan", "Italy", 0.79325, 0.15999},
        {"Montevideo", "Uruguay", -0.60883, -0.98029},
        {"Moscow", "Russia", 0.97302, 0.65624},
        // {"Munich", "Germany", 0.84009, 0.20217},
        {"Nagasaki", "Japan", 0.57247, 2.26806},
        {"Nagoya", "Japan", 0.61290, 2.38994},
        {"Nairobi", "Kenya", -0.02473, 0.64432},
        {"Nanjing (Nanking)", "China", 0.55938, 2.07491},
        {"Naples", "Italy", 0.71268, 0.24871},
        {"New Delhi", "India", 0.49887, 1.34739},
        {"Newcastle-on-Tyne", "England", 0.95935, -0.02822},
        {"Odessa", "Ukraine", 0.81071, 0.53756},
        {"Osaka", "Japan", 0.60272, 2.36492},
        {"Oslo", "Norway", 1.04632, 0.18675},
        {"Panama City", "Panama", 0.15650, -1.38812},
        {"Paramaribo", "Suriname", 0.10036, -0.96429},
        {"Paris", "France", 0.85172, 0.04072},
        {"Perth", "Australia", -0.55763, 2.02225},
        {"Plymouth", "England", 0.87994, -0.07127},
        {"Port Moresby", "Papua New Guinea", -0.16435, 2.56796},
        {"Prague", "Czech Republic", 0.87412, 0.25191},
        {"Rangoon", "Myanmar", 0.29380, 1.67552},
        {"Reykjavík", "Iceland", 1.11817, -0.38339},
        {"Rio de Janeiro", "Brazil", -0.40055, -0.75398},
        {"Rome", "Italy", 0.73129, 0.21729},
        {"Salvador", "Brazil", -0.22573, -0.67108},
        {"Santiago", "Chile", -0.58410, -1.23482},
        {"St. Petersburg", "Russia", 1.04603, 0.52883},
        {"São Paulo", "Brazil", -0.41044, -0.81187},
        {"Shanghai", "China", 0.54396, 2.11999},
        {"Singapore", "Singapore", 0.02153, 1.81369},
        {"Sofia", "Bulgaria", 0.74467, 0.40724},
        {"Stockholm", "Sweden", 1.03469, 0.31503},
        {"Sydney", "Australia", -0.59341, 2.63545},
        {"Tananarive", "Madagascar", -0.32870, 0.82990},
        {"Teheran", "Iran", 0.62396, 0.90321},
        {"Tokyo", "Japan", 0.62250, 2.43910},
        {"Tripoli", "Libya", 0.57509, 0.23038},
        {"Venice", "Italy", 0.79296, 0.21526},
        {"Veracruz", "Mexico", 0.33452, -1.67842},
        {"Vienna", "Austria", 0.84183, 0.28507},
        {"Vladivostok", "Russia", 0.75340, 2.30383},
        {"Warsaw", "Poland", 0.91164, 0.36652},
        {"Wellington", "New Zealand", -0.72053, 3.05054},
        {"Zürich", "Switzerland", 0.82641, 0.14864},
    };
    return cities;
};

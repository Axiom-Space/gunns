#include "../include/GunnSight.hh"

GunnSight::GunnSight()
{
    std::vector<std::string> row;
    std::string line, word;
    std::ifstream fin("Modified_data/Tuning.csv", std::ios::in);
    if(fin.is_open()) {
        std::cout << "File is open" << std::endl;
        getline(fin, line);
        while(getline(fin, line)) {
            row.clear();
            std::stringstream str(line);
            while(getline(str, word, ',')) {
                row.push_back(word);
            }
            std::cout << row.at(0) << " " << row.at(1) << " " << row.at(2) << std::endl;
            names.push_back(row.at(0));
            flow_rates.push_back(stod(row.at(1)));
            p_drops.push_back(stod(row.at(2)));
            conductivities.push_back(0.0001);
        }
    } else { 
        std::cout << "Could not open tuning file." << std::endl; 
    }
    system("rm Modified_data/out.txt");
    std::ofstream fout("Modified_data/out.txt", std::ios::out);
    fout << "------------- GunnSight Monitor -------------\n" << std::endl;
}

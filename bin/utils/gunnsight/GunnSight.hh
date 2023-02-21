/****************************************************************
PURPOSE: Trick optimization job for GUNNSight tuning

LIBRARY DEPENDENCY:
  ((GunnSight.o))
****************************************************************/
#ifndef GUNNSIGHT_HH
#define GUNNSIGHT_HH

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define _MC_EPSILON 0.1

class GunnSight
{
    public:
        GunnSight();
        template <class T> int update(T* network);

    private:
        std::vector<std::string> names;
        std::vector<double> p_drops;
        std::vector<double> flow_rates;
        std::vector<double> conductivities;
};

template <class T>
int GunnSight::update(T* network) {
    std::ofstream fout("Modified_data/out.txt", std::ios::app);
    double p_run, q_run, p_delta, q_delta, p_sigma, q_sigma;
    double g_prev, g_sigma;
    fout << "UPDATE ------------------------------- " << std::endl;
    for(int i=0; i<names.size(); i++) {
        p_delta = network->linkMap[names[i]]->getPotentialDrop() - p_drops.at(i);
        p_sigma = p_delta / (p_drops.at(i) * _MC_EPSILON);
        q_run = network->linkMap[names[i]]->getVolFlowRate();
        q_delta = q_run - flow_rates.at(i);
        q_sigma = q_delta  / (flow_rates.at(i) * _MC_EPSILON);
        g_prev = network->linkMap[names[i]]->getEffectiveConductivity();
        if(abs(q_sigma) > 1000) {
            fout << names[i] << "\n\tERROR---  q_sigma: " << q_sigma << "\n\t\t\t  p_sigma: " << p_sigma << std::endl;
        } else if(abs(q_sigma) + abs(p_sigma) > 1) {  // Pressure drop is too large and flow rate is too low
            g_sigma = q_sigma; // Edit this to change tuning function
            conductivities.at(i) = g_prev - g_prev * g_sigma * _MC_EPSILON;
            network->linkMap[names[i]]->mMaxConductivity = conductivities.at(i);
            fout << names[i] << "\n\tUPDATE--- q_run: " << network->linkMap[names[i]]->getVolFlowRate() << "\tq_goal: " << flow_rates.at(i)
                << "\tq_sigma: " << q_sigma << "\n\t\t\t  p_run: " << network->linkMap[names[i]]->getPotentialDrop() << "\tp_goal: " << p_drops.at(i)
                << "\tp_sigma: " << p_sigma << "\n\t\t\t  g_prev: " << g_prev << "\tg_new: " << network->linkMap[names[i]]->mMaxConductivity << std::endl;

        } else {
            fout << names[i] << "\n\tFINAL---  q_run: " << network->linkMap[names[i]]->getVolFlowRate() << "\tq_goal: " << flow_rates.at(i)
                << "\tq_sigma: " << q_sigma << "\n\t\t\t  p_run: " << network->linkMap[names[i]]->getPotentialDrop() << "\tp_goal: " << p_drops.at(i)
                << "\tp_sigma: " << p_sigma << "\n\t\t\t  g_final: " << g_prev << std::endl;
        }
    }
    fout << "--------------------------------------\n" << std::endl;
}

#endif
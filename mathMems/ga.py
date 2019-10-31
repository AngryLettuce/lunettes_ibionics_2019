import numpy as np
import matplotlib.pyplot as plt


def generateChild50(parent1, parent2):
    child1 = {}
    child2 = {}
    for i,key in enumerate(parent1.keys()):
        if i <= 3:
            child1[key] = parent1[key]
            child2[key] = parent2[key]
        else :
            child1[key] = parent2[key]
            child2[key] = parent1[key]

    return child1, child2


def angle2XY(theta_x, theta_y, x_incident, y_incident, z_incident, k):
    #z = 49.08029
    theta_x = np.deg2rad(theta_x)
    theta_y = np.deg2rad(theta_y)

    d = np.array([x_incident[0], y_incident[0], z_incident[0]])
    alpha = np.deg2rad(21) - theta_y + np.deg2rad(90)
    beta = theta_x
    z_n = np.sin(alpha)*np.cos(beta)
    x_n = np.cos(alpha)*np.cos(beta)
    y_n = np.sin(beta)
    N = [x_n[0],y_n[0],z_n[0]]
    n = np.array(N) / np.linalg.norm(N)
    r = d-2*np.dot(d, n)*n

   # k = z/r[2]
    z = k*r[2]
    x = k*r[0]
    y = k*r[1]
    return x,y,z


def cal_fitness(pop):
    #z_reel = 49.08029
    z_reel = 341.24
    fitness = []

    for genes in pop:
        theta_x = 3.92 # 0
        theta_y = -0.18 #0
        x_reel = -337.61
        y_reel = 34.63
        x,y,z = angle2XY(theta_x+genes['offset_theta_x'],theta_y+genes['offset_theta_y'],
                        genes['x'],genes['y'],genes['z'], genes['k1'])
        #d1 = np.sqrt((y-0)**2+(x--44.1921)**2+(z-z_theorique)**2)
        d1 = np.sqrt((y-y_reel)**2+(x-x_reel)**2+(z-z_reel)**2)
        theta_x = 0.42 # 1
        theta_y = -0.24 # -1
        x_reel = -337.61
        y_reel = -22.17
        x,y,z = angle2XY(theta_x+genes['offset_theta_x'],theta_y+genes['offset_theta_y'],
                        genes['x'],genes['y'],genes['z'], genes['k2'])
        #d2 = np.sqrt((y-2.2094)**2+(x--47.4164)**2+(z-z_theorique)**2)
        d2 = np.sqrt((y-y_reel)**2+(x-x_reel)**2+(z-z_reel)**2)
        theta_x = 0.2 #-1
        theta_y = 3.08 #1
        x_reel = -256
        y_reel = -22.17
        x,y,z = angle2XY(theta_x+genes['offset_theta_x'],theta_y+genes['offset_theta_y'],
                        genes['x'],genes['y'],genes['z'], genes['k3'])
        #d3 = np.sqrt((y--2.1026)**2+(x--41.1996)**2+(z-z_theorique)**2)
        d3 = np.sqrt((y-y_reel)**2+(x-x_reel)**2+(z-z_reel)**2)
        theta_x = 4.06
        theta_y = 3.16
        x_reel = -256
        y_reel = 34.63
        x,y,z = angle2XY(theta_x+genes['offset_theta_x'],theta_y+genes['offset_theta_y'],
                        genes['x'],genes['y'],genes['z'], genes['k3'])
        d4 = np.sqrt((y-y_reel)**2+(x-x_reel)**2+(z-z_reel)**2)
        d_tot = d1+d2+d3+d4
        fitness.append(1/d_tot) # on souhaite maximiser 1/d_tot
    return fitness


def select_mating_pool(pop, fitness, num_parents_mating):
    children = []
    cum_sum_fitness = np.cumsum(fitness[:-1])

    for i in range(num_parents_mating):
        if i < 100: # Elitisme
            child1, child2 = generateChild50(pop[i], pop[i+1])
            children.append(child1)
            children.append(child2)
        else:
            rand1 = np.random.uniform(0,1)
            rand2 = np.random.uniform(0,1)
            index1 = np.argmax(cum_sum_fitness>rand1)
            parent1 = pop[index1]
            index2 = np.argmax(cum_sum_fitness>rand2)
            parent2 = pop[index2]
            child1, child2 = generateChild50(parent1, parent2)
            children.append(child1)
            children.append(child2)
    return children


def generate_first_pop(sol_per_pop) :
    pop = []
    for i in range(sol_per_pop):
      genes = {
          'offset_theta_x': np.random.uniform(low=-1,high=1, size=1),
          #'offset_theta_x': np.array([0]),
          #'offset_theta_y': np.array([0]),
          'offset_theta_y': np.random.uniform(low=-1,high=1, size=1),
          'k1': np.random.uniform(low=300,high=600, size=1),
          'k2': np.random.uniform(low=300,high=600, size=1),
          'k3': np.random.uniform(low=300,high=600, size=1),
          'k4': np.random.uniform(low=300,high=600, size=1),
          'x': np.random.uniform(low=-0.1,high=0.1, size=1),
          'y': np.random.uniform(low=-0.1,high=0.1, size=1),
          'z': np.random.uniform(low=-1.1,high=-0.9, size=1)
          #'x': np.array([0]),
          #'y': np.array([0]),
          #'z': np.array([-1]),
          }
      pop.append(genes)
    return pop


def add_mutation(childs, prob_mutation) :
    for child in childs :
        random_mutation = np.random.uniform(0,1)
        if random_mutation < prob_mutation :
            random_dict_key = np.random.randint(0,9)

            if random_dict_key == 0 :
                child['offset_theta_x'] = child['offset_theta_x'] + np.random.normal(0, 0.01)
            elif random_dict_key == 1 :
                child['offset_theta_y'] = child['offset_theta_y'] + np.random.normal(0, 0.01)
            elif random_dict_key == 2 :
                child['k1'] = child['k1'] + np.random.normal(0, 0.1)
            elif random_dict_key == 3 :
                child['k2'] = child['k2'] + np.random.normal(0, 0.1)
            elif random_dict_key == 4 :
                child['k3'] = child['k3'] + np.random.normal(0, 0.1)
            elif random_dict_key == 5 :
                child['k4'] = child['k4'] + np.random.normal(0, 0.1)
            elif random_dict_key == 6 :
                child['x'] = child['x'] + np.random.normal(0, 0.01)
            elif random_dict_key == 7 :
                child['y'] = child['y'] + np.random.normal(0, 0.01)
            elif random_dict_key == 8 :
                child['z'] = child['z'] + np.random.normal(0, 0.01)
    return childs


def add_mutation2(childs, prob_mutation) :
    for child in childs :
        random_mutation = np.random.uniform(0,1)
        if random_mutation < prob_mutation :
            for random_dict_key in range(9):
        #    random_dict_key = np.random.randint(0,8)

                if random_dict_key == 0 :
                    child['offset_theta_x'] = child['offset_theta_x'] + np.random.normal(0, 0.01)
                elif random_dict_key == 1 :
                    child['offset_theta_y'] = child['offset_theta_y'] + np.random.normal(0, 0.01)
                elif random_dict_key == 2 :
                    child['k1'] = child['k1'] + np.random.normal(0, 0.01)
                elif random_dict_key == 3 :
                    child['k2'] = child['k2'] + np.random.normal(0, 0.01)
                elif random_dict_key == 4 :
                    child['k3'] = child['k3'] + np.random.normal(0, 0.01)
                elif random_dict_key == 5 :
                    child['k4'] = child['k4'] + np.random.normal(0, 0.01)
                elif random_dict_key == 6 :
                    child['x'] = child['x'] + np.random.normal(0, 0.01)
                elif random_dict_key == 7 :
                    child['y'] = child['y'] + np.random.normal(0, 0.01)
                elif random_dict_key == 8 :
                    child['z'] = child['z'] + np.random.normal(0, 0.01)
    return childs

def main():
    sol_per_pop = 20000 # 1500 fonctionne bien
    num_generations = 50
    num_parents_mating = int(sol_per_pop/2)

    pop = generate_first_pop(sol_per_pop)
    best_fitness = []
    average_fitness = []
    best_child = {}
    best_child_fitness = -1

    for generation in range(num_generations):
        # Measuring the fitness of each chromosome in the population.
        fitness = cal_fitness(pop)

        sorted_fitness = sorted(fitness)
        best_fitness.append(sorted_fitness[-1])
        average_fitness.append(np.mean(fitness))

        sorted_norm_fitness = np.array(sorted_fitness)/sum(sorted_fitness)
        # sorts pop in function of fitness
        sorted_pop = [p for _,p in sorted(zip(fitness, pop))]

        if sorted_fitness[-1] > best_child_fitness :
            best_child = sorted_pop[-1]
            best_child_fitness = sorted_fitness[-1]
        print('gen ' + str(generation) + ' best fitness : ' + str(1/sorted_fitness[-1]))
        # Selecting the best parents in the population for mating.
        children = select_mating_pool(sorted_pop, sorted_norm_fitness, num_parents_mating)
        # mutation
        add_mutation2(children, 0.5) # mutation2 fonctionnait bien
        # going back
        pop = children

    print(best_child)
    print(1/best_child_fitness)
    plt.plot(1/np.array(best_fitness))
    plt.plot(1/np.array(average_fitness))
    plt.show()

if __name__ == '__main__':
    main()
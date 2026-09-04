

import numpy as np
import random
from tqdm import tqdm


class Twiddle:

    def __init__(self, parameters, changes, threshold=0.5, max_iter=150, patience=15,
                 tolerance=0.0):
        """
        Twiddle is a quite simple algorithm that tries to minimize a function that returns any type of error.
        Essentially, it tries to search for the best parameters through small changes according to a change vector.

        Parameters
        ----------
        parameters: array-like
            The parameters you want to optmize
        changes: array-like
            The changes you want to allow for each parameter. If you don't know exactly, use 1 for each of then. It must
            have the same shape of `parameters`
        threshold: float
            A convergence threshold to stop the optimization
        max_iter: int
            The number of maximum iterations
        """

        if len(parameters) != len(changes):
            raise Exception("parameters and changes must have the same length!")

        self.parameters = parameters
        self.changes = changes
        self.threshold = threshold
        self.max_iter = max_iter
        self.patience = patience
        self.tolerance = tolerance

    def minimize(self, fit_func, config=None, show_parameters=None):
        """
        This method performs the minimization.
        Parameters
        ----------
        fit_func: callback function
            The function to be optimized. It must get as parameters the `parameter` array and a set of configuration
            that you may design as you want
        config: None or any
            The configuration to pass as parameters to the function.
        show_parameters: None or int
            If it's not None, it prints the current state of the parameters every `show_parameters` iterations.

        Returns
        -------
        parameters: array-like
            The optimized parameters
        best_error: float
            The minimum error found
        """

        iter_ = 1
        config["iteration"] = iter_
        best_error = fit_func(self.parameters, config)
        best_error_ever = best_error
        iter_wo_improvement = 0
        with tqdm(total=self.max_iter, ascii=True, desc="Twiddle", ncols=120) as tq:

            convergence = sum(self.changes)
            while convergence > self.threshold:

                for i in range(len(self.parameters)):
                    if self.changes[i] == 0:
                        continue
                    self.parameters[i] += self.changes[i]
                    error = fit_func(self.parameters, config)

                    if error < best_error:  # There was some improvement
                        best_error = error
                        self.changes[i] *= 1.1
                    else:  # There was no improvement
                        self.parameters[i] -= 2 * self.changes[i]  # Go into the other direction
                        error = fit_func(self.parameters, config)

                        if error < best_error:  # There was an improvement
                            best_error = error
                            self.changes[i] *= 1.05
                        else:  # There was no improvement
                            self.parameters[i] += self.changes[i]
                            # As there was no improvement, the step size in either
                            # direction, the step size might simply be too big.
                            self.changes[i] *= 0.95

                iter_wo_improvement += 1
                best_diff_ = round(best_error, 4) - round(best_error_ever, 4)
                if best_diff_ < 0:
                    best_error_ever = best_error
                    if best_diff_ < -self.tolerance:
                        iter_wo_improvement = 0

                if iter_wo_improvement >= self.patience:
                    print("\n- Patience is over!")
                    break

                if show_parameters is not None:
                    if iter_ % show_parameters == 0:
                        print("\n- Best error so far: {:.3f}".format(best_error_ever))
                        print("- Paramenters:", np.round(self.parameters, 2))
                        print("- Changes:", np.round(self.changes, 2))
                        print(f"- Patience: {iter_wo_improvement}/{self.patience}")

                if iter_ >= self.max_iter:
                    break

                iter_ += 1
                config["iteration"] = iter_
                convergence = sum(self.changes)

                # Updating tqdm
                tq.set_postfix(best_error='{:05.3f}'.format(best_error),
                               convergence='{:.3f}'.format(convergence),
                               patience=f'{iter_wo_improvement}')
                tq.update()

        return self.parameters, best_error



class DiffEvolution:

    def __init__(self, population=None, cross_rate=0.85, mut_rate=0.5, min_error=0, max_iter=130,
                 constraint=None,
                 patience=10, patience_decay=20, tolerance=0.1):
        """
        The Differential Evolution (DE) algorithm is a fast evolutionary algorithm used to minimize any type of function
        without having a knowledge about it.

        Parameters
        ----------
        population: None or np.ndarray
            The population to be used during the optimization. If it's None, you must call the `initialize_population`
            population method before use the minimization one.
        cross_rate: float
            The crossover rate
        mut_rate: float
            The mutation rate
        min_error: float
            The minimum error that we can stop the optimization
        max_iter: int
            The maximum number of iterations
        constraint: None or array-like
            A constraint array to ensure that each parameter in the population lies in the correnct bounds
        patience: int
            Number of consecutive iterations to stop minimization
        patience_decay: int
            The patient decreases by 1 every patience_decay iteration
        tolerance: float:
            The tolerance to determine if the method get a improvement
        """
        self.population = population
        self.cross_rate = cross_rate
        self.mut_rate = mut_rate
        self.min_error = min_error
        self.max_iter = max_iter
        self.constraint = constraint
        self.best_solution = None
        self.error_history = list()
        self.paticence = patience
        self.tolerance = tolerance
        self.paticence_decay = patience_decay

    def initialize_population(self, len_pop, bounds, kind='uniform', heritage_pop=None,
                              heritage_dev=1):
        """
        This method initializes the population according to the parameters
        Parameters
        ----------
        len_pop: int
            The number of individuals within the population. Each individual must have the number of parameters to be
            minimized. For example, if I'm trying to optimize a function that has 3 parameters using a population of 5
            individuals, `population` must have shape equal to [5, 3].
        bounds: list of tuples
            The boundaries for each parameter within the population. It must match the number of parameters. For the
            above example, bounds must be: [(a1, b1), (a2, b2), (a3, b3)]
        kind: {"uniform", "normal"}
            The type of distribution to sample the population. If you set `uniform` the bounds (a,b) are used as the
            max and min values of the uniform distribution. On the other hand, if you use `normal`, they are used as the
            mean and std for the normal distribution
        heritage_pop: None or array-like
            If you have a good individual (i.e., an array of parameters) that you think all other individual should
            be created from, you can pass it as parameters. Important: if you do it, bound and kind are not used.
        heritage_dev: float
            If `heritage_pop` is not None, it's used to as a factor of spread to the individuals created from
            `heritage_dev`.
        Returns
        -------
        It fills the self.population with the created population.
        """

        if len_pop < 4:
            raise Exception("- The population must be greater than 3!")

        if heritage_pop is not None:
            population = np.zeros([len_pop, len(heritage_pop)])
            heritage_pop = np.asarray(heritage_pop)
            for i in range(len_pop):
                population[i] = heritage_pop + (
                            heritage_pop * np.random.rand(len(heritage_pop)) * heritage_dev)

        else:
            _dim = len(bounds)
            population = np.zeros([len_pop, len(bounds)])
            if kind == 'normal':
                for j in range(_dim):
                    b = bounds[j]
                    population[:, j] = np.random.normal(b[0], b[1], len_pop)
            elif kind == 'uniform':
                for j in range(_dim):
                    b = bounds[j]
                    population[:, j] = np.random.uniform(b[0], b[1], len_pop)
            else:
                raise Exception(f"- There is no kind = {kind} available on initialize_population!")

        self.population = population

    def _apply_constraint(self, indv):
        """
        Private function to apply the constraint to the individuos
        """
        if self.constraint is None:
            return indv

        for i in range(len(indv)):
            if indv[i] < self.constraint[i][0]:
                indv[i] = self.constraint[i][0]
            elif indv[i] > self.constraint[i][1]:
                indv[i] = self.constraint[i][1]

        return indv

    def _mutation(self):
        """
        This private method performs the mutation operator. In brief, it combines individuals from the same population
        according to the mutation rate. It results in a mutate population (mut_pop)
        """
        _pop_len = len(self.population)
        mut_pop = np.zeros_like(self.population)
        for k in range(_pop_len):
            indices = list(range(0, _pop_len))
            selected_indices = random.sample(indices, 3)

            indv_1 = self.population[selected_indices[0]]
            indv_2 = self.population[selected_indices[1]]
            indv_3 = self.population[selected_indices[2]]

            mut_pop[k] = indv_3 + self.mut_rate * (indv_2 - indv_1)

        return mut_pop

    def _crossover(self, mut_pop):
        """
        This private method performs the crossover operation. In brief, it cross the individuals between the mutate
        population (mut_pop) and the current one (self.population), which results in the cross population (cross_pop).
        """
        cross_pop = np.zeros_like(self.population)
        _pop_len, _dim_pop = self.population.shape
        _cross_rate = np.array([self.cross_rate] * _dim_pop)
        for k in range(_pop_len):
            cross_pop[k] = np.where(np.random.rand(_dim_pop) < _cross_rate, mut_pop[k],
                                    self.population[k])

        return cross_pop

    def _selection(self, cross_pop, fit_func, config, it):
        """
        This private method performs the selection operation. In brief, it selects the best individuals from the cross
        population and the current one. The selection is based on the fit_func and only the fittest individuals survives
        """

        _pop_len = len(self.population)
        selected_pop = np.zeros_like(self.population)
        best_error = np.inf
        best_individuo = None

        with tqdm(total=_pop_len, ascii=True, desc=f"Selection for iteration {it}",
                  ncols=100) as tq:
            for k in range(_pop_len):
                error_pop = fit_func(self.population[k], config)
                error_cross_pop = fit_func(cross_pop[k], config)

                if error_pop <= error_cross_pop:
                    selected_pop[k] = self.population[k]
                    if error_pop <= best_error:
                        best_error = error_pop
                        best_individuo = selected_pop[k]
                else:
                    selected_pop[k] = cross_pop[k]
                    if error_cross_pop <= best_error:
                        best_error = error_cross_pop
                        best_individuo = selected_pop[k]

                # Updating tqdm
                tq.set_postfix(best_error='{:05.3f}'.format(best_error))
                tq.update()

        return selected_pop, best_individuo, best_error

    def minimize(self, fit_func, config):
        """
        This method performs the minimization in a fast way. Mutation, crossover, and selection are performed inside
        the method

        Parameters
        ----------
        fit_func: callback function
            The function to be optimized. It must get as parameters the `parameter` array and a set of configuration
            that you may design as you want
        config: None or any
            The configuration to pass as parameters to the function.

        Returns
        -------
        best_solution: array-like
            The optimized parameters
        best_error: float
            The minimum error found
        """

        print("-" * 50)
        print("- Start Differential Evolution minimization")
        print("-" * 50)

        if self.population is None:
            raise Exception(
                "- You must fulfill the population before call the minimization method. Please, check" +
                " the initialize_population method!")

        # iterations
        _pop_len = len(self.population)
        best_error_ever = np.inf
        iter_wo_improvement = 0
        for it in range(1, self.max_iter):

            iter_wo_improvement += 1
            config["iteration"] = it
            mut_pop = self._mutation()
            cross_pop = self._crossover(mut_pop)
            new_pop, best_individuo, best_error = self._selection(cross_pop, fit_func, config, it)
            self.error_history.append(best_error)
            self.population = new_pop
            self.best_solution = best_individuo

            best_diff_ = round(best_error, 4) - round(best_error_ever, 4)
            if best_diff_ < 0:
                best_error_ever = best_error
                if best_diff_ < -self.tolerance:
                    iter_wo_improvement = 0

            if best_error <= self.min_error:
                print("\n- Min error achieved!")
                break

            if iter_wo_improvement >= self.paticence:
                print("\n- Patience is over!")
                break

            if self.paticence_decay is not None:
                if self.paticence > 2 and it > 1:
                    if it % self.paticence_decay == 0:
                        self.paticence -= 1

            print("-" * 50)
            print(f"- Iteration {it}/{self.max_iter} overview:")
            print("-" * 50)
            print("- Best error so far: {:.3f}".format(best_error_ever))
            print("- Parameters:", np.round(self.best_solution, 2))
            print(f"- Patience: {iter_wo_improvement}/{self.paticence}")
            print("-" * 50)

        return self.best_solution, best_error_ever

    def minimize_fast(self, fit_func, config, show_parameters=None):
        """
        This method performs the minimization. It's based on three operations: mutation, crossover, and selection.

        Parameters
        ----------
        fit_func: callback function
            The function to be optimized. It must get as parameters the `parameter` array and a set of configuration
            that you may design as you want
        config: None or any
            The configuration to pass as parameters to the function.

        Returns
        -------
        best_solution: array-like
            The optimized parameters
        best_error: float
            The minimum error found
        """

        print("-" * 50)
        print("- Start Differential Evolution fast minimization")
        print("-" * 50)

        if self.population is None:
            raise Exception(
                "- You must fulfill the population before call the minimization method. Please, check" +
                " the initialize_population method!")

        # iterations
        _pop_len, _dim_pop = self.population.shape
        _cross_rate = np.array([self.cross_rate] * _dim_pop)
        best_patience, best_error = np.inf, np.inf
        iter_wo_improvement = 0

        with tqdm(total=self.max_iter, ascii=True, desc="DE", ncols=100) as tq:
            for it in range(1, self.max_iter):

                selected_pop = np.zeros_like(self.population)
                iter_wo_improvement += 1
                config["iteration"] = it
                for kp in range(_pop_len):

                    # mutation
                    indices = list(range(0, _pop_len))
                    indices.remove(kp)
                    selected_indices = random.sample(indices, 3)
                    indv_1 = self.population[selected_indices[0]]
                    indv_2 = self.population[selected_indices[1]]
                    indv_3 = self.population[selected_indices[2]]
                    indv_curr = self.population[kp]

                    indv_mut = indv_3 + self.mut_rate * (indv_2 - indv_1)
                    indv_mut = self._apply_constraint(indv_mut)

                    # crossover
                    indv_cross = np.where(np.random.rand(_dim_pop) < _cross_rate, indv_mut,
                                          indv_curr)

                    # selection
                    error_pop = fit_func(indv_curr, config)
                    error_cross = fit_func(indv_cross, config)

                    if error_pop < error_cross:
                        selected_pop[kp] = indv_curr
                        if error_pop < best_error:
                            best_error = error_pop
                            self.best_solution = selected_pop[kp]
                    else:
                        selected_pop[kp] = indv_cross
                        if error_cross < best_error:
                            best_error = error_cross
                            self.best_solution = selected_pop[kp]

                # updating the epochs without improviment
                best_diff_ = round(best_error, 4) - round(best_patience, 4)
                if best_diff_ < -self.tolerance:
                    best_patience = best_error
                    iter_wo_improvement = 0

                if best_error <= self.min_error:
                    print("\n- Min error achieved!")
                    break

                if iter_wo_improvement >= self.paticence:
                    print("\n- Patience is over!")
                    break

                if self.paticence_decay is not None:
                    if self.paticence > 2 and it > 1:
                        if it % self.paticence_decay == 0:
                            self.paticence -= 1

                if show_parameters is not None:
                    if it % show_parameters == 0:
                        print(f"\n- Iteration {it}/{self.max_iter} overview:")
                        print("-- Parameters:", np.round(self.best_solution, 2))

                # Updating population
                self.population = selected_pop

                # Updating tqdm
                tq.set_postfix(best_error='{:05.3f}'.format(best_error),
                               patience=f'{iter_wo_improvement}')
                tq.update()

        return self.best_solution, best_error

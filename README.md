# philosophers
project for ecole 42

Dining philosophers problem is a problem created by Edsger Wybe Dijkstra in 1965 to explain the deadlock state  
of an operating system, which is traditionally commonly introduced in lectures on operating systems.  
You can read about it [here](https://en.wikipedia.org/wiki/Dining_philosophers_problem).
  
### Rules for philosophers:  
* One or more philosophers sit at a round table.
  There is a large bowl of spaghetti in the middle of the table.
* The philosophers alternatively eat, think, or sleep.
  While they are eating, they are not thinking nor sleeping; while thinking, they are not eating nor sleeping;
  and, of course, while sleeping, they are not eating nor thinking.
* There are also forks on the table. There are as many forks as philosophers.
* Because serving and eating spaghetti with only one fork is very inconvenient, a
  philosopher takes their right and their left forks to eat, one in each hand.
* When a philosopher has finished eating, they put their forks back on the table and start sleeping.
  Once awake, they start thinking again. The simulation stops when a philosopher dies of starvation.
* Every philosopher needs to eat and should never starve.
* Philosophers don’t speak with each other.
* Philosophers don’t know if another philosopher is about to die.
* No need to say that philosophers should avoid dying!
  
In the mandatory part, each philosopher must be a thread,  
but in the bonus part every philosopher must be a process!  
  
### Arguments:
• number_of_philosophers
• time_to_die
• time_to_eat
• time_to_sleep
• [number_of_times_each_philosopher_must_eat]
  
You can see the full assignment here [subject](https://github.com/cbridget42/philosophers_42/blob/main/subject/en.subject.pdf)  
Coding style: [norminette](https://github.com/cbridget42/philosophers_42/blob/main/subject/en.norm.pdf)  
  
### Usage:
* first you need to download the repository and compile the code! To do this, run:
	```bash
	git clone git@github.com:cbridget42/philosophers_42.git
	cd philosophers_42/philo
    or
    cd philosophers_42/philo_bonus
	make
	```
* then you will have an executable file. Exemple usage:
    ```bash
	./philo 5 800 200 200 7
	```
	```bash
    ./philo 4 410 200 200
	```
* also you can:
	```bash
	make clean //remove objects files
	make fclean //remove objects files and executable
	```
  
#### Usage example:
![image](https://github.com/cbridget42/philosophers_42/blob/main/images/phylo.svg)
  
#### Tests:
![image](https://github.com/cbridget42/philosophers_42/blob/main/images/Screenshot%20from%202022-11-16%2018-34-56.png)
  
#### final score:
![image](https://github.com/cbridget42/philosophers_42/blob/main/images/Screenshot%20from%202022-11-16%2018-47-26.png)
  
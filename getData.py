"""Python script for getting data of NeuronProject."""
# Python script file
# Author : Hadrien Renaud-Lebret
# Created on 2016-11-26


# ********************************* Imports ***********************************
# Imports :
import subprocess as sp

# *********************************** Data ************************************
# Data :
ranges = {
    'momentum': [0.1],
    'maximal_distance': [0.1],
    'max_limit_loop': [200],
    'length_alphabet': [52],
}


# ******************************** Functions **********************************
# Functions :

def exec_NeuronProject(*cmds, timeout=None, commands=True):
    """Execute NeuronProject.

    cmds : commandes de NeuronProject
    timeout : timeout argument of subprocess.run
    """
    if commands:
        commands_list = ['-c']
    else:
        commands_list = []
    output = sp.run(['./NeuronProject'] + commands_list + list(cmds),
                    timeout=timeout, stdout=sp.PIPE).stdout
    return output.decode('UTF-8')


def set_networks_settings(**args):
    """Set NeuronProject.cfg with the right settings.

    Settings available can be found in NeuronProject.cfg.default
    """
    default_file = open('NeuronProject.cfg.default', 'r')
    config_file = open('NeuronProject.cfg', 'w')
    for line in default_file:
        li = line.split()
        if len(li) == 3 and li[1] == '=' and li[0] in args.keys():
            li[2] = str(args[li[0]])
            line = ' '.join(li)
            print(line)
        config_file.write(line)
    default_file.close()
    config_file.close()


def print_file(file_name):
    """Print a file."""
    print("\n\nFILE : " + file_name + '\n')
    for line in open(file_name):
        print(line, end='')
    print("\nEND.")


# ***************************** Excecutable code ******************************
# Excecutable code :

if __name__ == '__main__':
    print(exec_NeuronProject('save'))
    print_file('NeuronProject.cfg')
    set_networks_settings(max_limit_loop=450)
    print_file('NeuronProject.cfg')

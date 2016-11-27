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

def exec_NeuronProject(commands, timeout=None, is_commands=True):
    """Execute NeuronProject.

    commands : commandes de NeuronProject
    timeout : timeout argument of subprocess.run
    """
    if is_commands:
        commands_list = ['-c']
    else:
        commands_list = []
    output = sp.run(['./NeuronProject'] + commands_list + list(commands),
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


def get_result(commands, timeout=None, is_commands=True):
    """Launch NeuronProject and analyse output string to get data."""
    output = exec_NeuronProject(commands, timeout=timeout, is_commands=is_commands)
    lines = output.split('\n')
    if len(commands) == 0:
        return False
    commands_output = [[] for c in commands]  # commands output
    pre_output = []  # lines before commands
    post_output = []  # lines after commands
    good_ending = False
    l = 0  # line number
    while l < len(lines) and lines[l] != "Command 0 : " + commands[0]:
        pre_output.append(lines[l])
        l += 1
    for i, command in enumerate(commands):
        if i < len(commands) - 1:
            while l < len(lines) and lines[l] != "Command {} : ".format(i + 1, commands[i + 1]):
                commands_output[i].append(lines[l])
                l += 1
        else:
            while l < len(lines) and lines[l] != 'End commands':
                commands_output[i].append(lines[l])
                good_ending = True
                l += 1
            l += 1
    while l < len(lines):
        post_output.append(lines[l])
        l += 1
    if not good_ending:
        return False
    return (pre_output, commands_output, post_output)


def get_data_save(output):
    """Process data from save command."""
    return output


def get_data_test(output):
    """Process data from test command."""
    return output


def get_data_learn(output):
    """Process data from learn command."""
    return output


def get_data_filter(output):
    """Process data from filter command."""
    return output


def get_data_database(output):
    """Process data from database command."""
    return output


def get_data_new(output):
    """Process data from new command."""
    return output


def get_data(commands, commands_output):
    """Get data from results."""
    dico = {
        'learn': get_data_learn,
        'save': get_data_save,
        'test': get_data_test,
        'filter': get_data_filter,
        'database': get_data_database,
        'new': get_data_new,
    }
    data = [0 for i in range(len(commands))]
    for i in range(len(commands)):
        data[i] = dico[commands[i]](commands_output[i])
    return data

# ***************************** Excecutable code ******************************
# Excecutable code :

if __name__ == '__main__':
    commands = ['save']
    print(get_data(commands, get_result(commands)[1]))

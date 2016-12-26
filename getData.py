"""Python script for getting data of NeuronProject."""
# Python script file
# Author : Hadrien Renaud-Lebret
# Created on 2016-11-26


# ********************************* Imports ***********************************
# Imports :
import subprocess as sp
import re
import json

# *********************************** Regex ***********************************
# Regex + links to regex101.com where explanations are stocked

regexp = {
    # Regex for matching different parts of the learning output
    "ApprentissageLettre": re.compile(r"Apprentissage de la lettre (?P<letter>\S) \.\.\."),


    # Regex for matching learning output :
    # https://regex101.com/r/kBplnY
    "learningProductif": re.compile(
        r"Apprentissage productif : (?P<number_examples_read>[0-9]+) exemples lus sur" + \
        r" (?P<number_examples_total>[0-9]+) avec (?P<number_succes>[0-9]+) succes, effectue" +\
        r" en (?P<time>[0-9]+.[0-9]*) secondes."),
    # https://regex101.com/r/7aemyh
    "learningAverageDistance": re.compile(
        r"Distance moyenne sur les exemples : (?P<average_distance>[0-9]+.?[0-9]*)"),
    # https://regex101.com/r/oLAg68
    "learningFailed": re.compile(
        r"Apprentissage INFRUCTUEUX sur (?P<number_examples_total>[0-9]+) exemples lus\. avec " + \
        r"(?P<number_succes>[0-9]+) succes, effectue en (?P<time>[0-9]+.?[0-9]*) secondes\.")
}

# *********************************** Data ************************************
# Data :
ranges = {
    'momentum': [0.1],
    'maximal_distance': [0.5],
    'max_limit_loop': [10, 50],
    'length_alphabet': [26],
    'slope': [1],
}

data_file_name = "data.json"


# ********************************** Classes **********************************
# Classes :

class IterRange:
    """Iterable object itering on a cartesian product different length."""

    def __init__(self, lengths):
        """Initialisor.

        lengths is the lengths of the sets on which we are itering.
        """
        self.lengths = lengths

    def __iter__(self):
        """Iterator."""
        self.position = [0 for i in self.lengths]
        if self.position:
            self.position[0] = -1
        return self

    def __next__(self):
        """Next method."""
        for i, l in enumerate(self.lengths):
            self.position[i] += 1
            if self.position[i] >= l:
                self.position[i] = 0
            else:
                break
        else:
            raise StopIteration
        return self.position


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
            while l < len(lines) and lines[l] != "Command {} : {}".format(i + 1, commands[i + 1]):
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
    if len(output) > 1 and output[1] == 'Sauvegarde des réseaux ... Réseaux sauvegardés !':
        return True
    else:
        return False


def get_data_test(output):
    """Process data from test command."""
    if len(output) > 4 and output[3] == 'Test effectué !' and output[4] != '-nan':
        return float(output[4])
    else:
        return False


def splitter_letters_learn(output):
    """Split output in letters."""
    output_splited = {'pre': []}
    letter = 'pre'
    for line in output:
        l = regexp['ApprentissageLettre'].match(line)
        if l:
            letter = l.group('letter')
            output_splited[letter] = [line]
        output_splited[letter].append(line)
    return output_splited


def get_data_learn_single(output):
    """Process data from one letter."""
    result = {}
    for line in output:
        if regexp['learningProductif'].match(line):
            result.update(regexp['learningProductif'].match(line).groupdict())
            result['succes'] = True
        elif regexp['learningAverageDistance'].match(line):
            result.update(regexp['learningAverageDistance'].match(line).groupdict())
        elif regexp['learningFailed'].match(line):
            result.update(regexp['learningFailed'].match(line).groupdict())
            result['succes'] = False
    if 'succes' not in result:
        result['succes'] = False
    return result


def get_data_learn(output):
    """Process data from learn command."""
    output_splited = splitter_letters_learn(output)
    result = {'succes': True, 'number_succes': 0}
    keys = ['succes', 'number_succes', 'average_distance',
            'number_examples_read', 'number_examples_total', 'time']
    result['keys'] = keys
    for letter, output_l in output_splited.items():
        if letter != 'pre':
            result[letter] = get_data_learn_single(output_l)
            if result[letter]['succes']:
                result['number_succes'] += 1
            else:
                result['succes'] = False
            result[letter] = [result[letter][k] for k in keys]
    return result


def get_data_filter(output):
    """Process data from filter command."""
    return 0


def get_data_database(output):
    """Process data from database command."""
    return 0


def get_data_new(output):
    """Process data from new command."""
    return 0


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


def process_data(key, keys):
    """Function calling the networks and getting the data for the parameters."""
    set_networks_settings(**dict(zip(keys, key)))
    commands = ['new', 'learn', 'test']
    output = get_result(commands)[1]
    return get_data(commands, output)


def read_data_file(file_name=data_file_name):
    """Function reading the data file.

    The file must be written in JSON.
    The function construct a dictionnary with the list :
    [ [[input],[output1],[output2], ... ],
      [[input],[output1],[output2], ... ],
      ...
    ]
    """
    data = []
    with open(file_name) as data_file:
        data = json.load(data_file)
    assert type(data) is list, "Load of JSON file failed, not compatible."
    dico = {}
    for elt in data:
        assert type(elt) is list and len(elt) > 1, "Load of JSON file failed, not compatible."
        dico[str(elt[0])] = elt[1:]
    return dico


def write_data_file(dico, file_name=data_file_name):
    """Function writing the dico in the file specified.

    The dico must be of type :
    {
        [input]:[[output1],[output2] ... ],
        [input]:[[output1],[output2] ... ],
        ...
    }
    The file will be overwritten.
    Return True if successful, False otherwise.
    """
    data = []
    for inp, out in dico.items():
        data.append([inp] + out)
    try:
        data_file = open(file_name, 'w')
        json.dump(data, data_file)
        data_file.close()
    except:
        return False
    return True


def command_data(repet=2, file_name=data_file_name, ranges=ranges):
    """Function commanding the data.

    Data situated in the file 'file_name' will be completed with every possible key in the cartesian
    product of the sets defined in ranges, repet times.
    The function cal for process_data on every key.
    """
    dico = read_data_file(file_name)
    keys = list(ranges.keys())
    keys.sort()
    for position in IterRange([len(ranges[k]) for k in keys]):
        print(position)
        key = [ranges[keys[i]][position[i]] for i in range(len(position))]
        json_key = json.dumps(key)
        if json_key not in dico:
            dico[json_key] = [process_data(key, keys)]
        else:
            while len(dico[json_key]) < repet + 1:
                dico[json_key].append(process_data(key, keys))
    write_data_file(dico, file_name)


# ***************************** Excecutable code ******************************
# Excecutable code :

if __name__ == '__main__':
    command_data()

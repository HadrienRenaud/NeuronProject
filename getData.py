"""Python script for getting data of NeuronProject."""
# Python script file
# Author : Hadrien Renaud-Lebret
# Created on 2016-11-26


# ********************************* Imports ***********************************
# Imports :
from subprocess import PIPE, run
from re import compile
from multiprocessing import Pool
import json

# *********************************** Regex ***********************************
# Regex + links to regex101.com where explanations are stocked

regexp = {
    # Regex for matching different parts of the learning output
    "ApprentissageLettre": compile(r"Apprentissage de la lettre (?P<letter>\S) \.\.\."),


    # Regex for matching learning output :
    # https://regex101.com/r/kBplnY
    "learningProductif": compile(
        r"Apprentissage productif : (?P<number_examples_read>[0-9]+) exemples lus sur" + \
        r" (?P<number_examples_total>[0-9]+) avec (?P<number_succes>[0-9]+) succes, effectue" +\
        r" en (?P<time>[0-9]+.[0-9]*) secondes."),
    # https://regex101.com/r/7aemyh
    "learningAverageDistance": compile(
        r"Distance moyenne sur les exemples : (?P<average_distance>[0-9]+.?[0-9]*)"),
    # https://regex101.com/r/oLAg68
    "learningFailed": compile(
        r"Apprentissage INFRUCTUEUX sur (?P<number_examples_total>[0-9]+) exemples lus\. avec " + \
        r"(?P<number_succes>[0-9]+) succes, effectue en (?P<time>[0-9]+.?[0-9]*) secondes\.")
}

# *********************************** Data ************************************
# Data :

# data to collect
ranges = {
    'momentum': [0],
    'maximal_distance': [0.5],
    'max_limit_loop': [10, 50],
    'length_alphabet': [26],
    'slope': [1],
}

# name of the data file in which is written the data
data_file_name = "data.json"

# number of instances calling NeuronProject simultaneously
processes = 5

# ********************************** Classes **********************************
# Classes :


class IterRange:
    """Iterable object itering on a cartesian product different length."""

    def __init__(self, lengths, verbose=False):
        """Initialisor.

        lengths is the lengths of the sets on which we are itering.
        """
        self.verbose = verbose
        self.lengths = lengths
        self.length = 1
        for elt in self.lengths:
            self.length *= elt

    def __iter__(self):
        """Iterator."""
        self.position = [0 for i in self.lengths]
        self.pos = 0
        if self.position:
            self.position[0] = -1
        return self

    def __next__(self):
        """Next method."""
        if self.verbose:
            # print("Avancement : {:.2%} \r".format(self.pos / self.length))
            self.pos += 1
        for i, l in enumerate(self.lengths):
            self.position[i] += 1
            if self.position[i] >= l:
                self.position[i] = 0
            else:
                break
        else:
            raise StopIteration
        return self.position

    def get_pos(self):
        """Method returns pos."""
        return self.pos

    def get_length(self):
        """Method returns length."""
        return self.length


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
    output = run(['./NeuronProject'] + commands_list + list(commands),
                 timeout=timeout, stdout=PIPE).stdout
    return output.decode('UTF-8')


def set_networks_settings(verbose=False, **args):
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
            if verbose:
                print(line)
        config_file.write(line)
    default_file.close()
    config_file.close()


def get_result(commands, timeout=None, is_commands=True, verbose=False):
    """Launch NeuronProject and analyse output string to get data."""
    output = exec_NeuronProject(commands, timeout=timeout, is_commands=is_commands)
    if verbose:
        print(output)
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
    if len(output) > 5 and output[4] == 'Test effectué !' and output[5] != '-nan':
        return float(output[5])
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
            result[letter] = {
                'succes': None, 'number_succes': None, 'average_distance': None,
                'number_examples_read': None, 'number_examples_total': None, 'time': None
            }
            result[letter].update(get_data_learn_single(output_l))
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


def process_data(key, keys, verbose=False):
    """Function calling the networks and getting the data for the parameters."""
    set_networks_settings(verbose=verbose, **dict(zip(keys, key)))
    commands = ['new', 'learn', 'test']
    # commands = ['new', 'test']
    result = get_result(commands, verbose=verbose)
    if type(result) is tuple and len(result) > 1:
        return get_data(commands, result[1])
    else:
        print(type(result))
        return False


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
        assert type(elt) is list and len(elt) > 0, "Load of JSON file failed, not compatible."
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


def command_data(repet=20, file_name=data_file_name, ranges=ranges,
                 verbose=False, processes=processes):
    """Function commanding the data.

    Data situated in the file 'file_name' will be completed with every possible key in the cartesian
    product of the sets defined in ranges, repet times.
    The function cal for process_data on every key.
    The function use the number specified in processes to get the data.
    """
    # Data dictionnary, heavy.
    dico = read_data_file(file_name)
    # Keys to call process_data
    keys = sorted(ranges.keys())
    # results of the execution of pool.apply_async
    async_results = []
    # IterRange object used here
    iterateur = IterRange([len(ranges[k]) for k in keys], verbose=not verbose)

    # In order to optimize the time of the execution, multiprocessing will be used here.
    # The argumement of Pool is the number of processes that are running simultaneously.
    # With less than 8 cpus, reduce that number.
    with Pool(processes=processes) as pool:

        # For each element of the cartesian product in ranges :
        for position in iterateur:
            key = [ranges[keys[i]][position[i]] for i in range(len(position))]

            # the json_key is the str used to hash key in a dictionnary
            json_key = json.dumps(key)

            # if key is not referenced in dico, we create an entry
            if json_key not in dico:
                dico[json_key] = []

            # then we add to this entry the required number of output
            for i in range(repet + 1 - len(dico[json_key])):

                if verbose:  # if required, we print some stuff...
                    print("Avancement : {:.2%} \r".format(
                        (iterateur.get_pos() + i / repet) / (iterateur.get_length())))

                # We apply process_data to the key, with the required argumements.
                # The function is called by a child of the current process, allowing a certain
                # number of function to be called simultaneously.
                #  The result is then added to the dico entry
                async_results.append(
                    pool.apply_async(process_data, (key, keys), kwds={'verbose': verbose},
                                     callback=dico[json_key].append))

        # We wait for the processes to finish.
        for res in async_results:
            res.get()

    # We write the data in the specified file.
    write_data_file(dico, file_name)


# ***************************** Excecutable code ******************************
# Excecutable code :

if __name__ == '__main__':

    def printb(string):
        """Print overide in a file."""
        with open('getData.log', 'a') as f:
            f.write(string)

    print = printb

    command_data(repet=1, verbose=False)

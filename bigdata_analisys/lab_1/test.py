from lab_1 import main as m


def test_suite_1():
    input_data = 'A'
    output_data = m.obfuscate(input_data)
    assert input_data != output_data


def test_suite_2():
    input_data = 'A'
    output_data = m.obfuscate(input_data)
    output_data = m.deobfuscate(output_data)
    assert input_data == output_data


def test_suite_3():
    input_data_1 = 'A'
    input_data_2 = 'B'
    output_data_1 = m.obfuscate(input_data_1)
    output_data_2 = m.obfuscate(input_data_2)
    assert output_data_1 != output_data_2

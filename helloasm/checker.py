import random
import subprocess


MIN_N = 10**3
MAX_N = 10**1000

SOLVE_FILE = './mul'


def run_solve(a, b):
    try:
        res = subprocess.run([SOLVE_FILE,], stdout=subprocess.PIPE, input='{}\n{}\n'.format(str(a), str(b)), encoding='ascii').stdout
    except Exception as e:
        print(e)
        return -1
    return res


def main():
    tests_num = 0
    while True:
        tests_num += 1
        a, b = random.randint(MIN_N, MAX_N), random.randint(MIN_N, MAX_N)
        if a * b != int(run_solve(a, b)):
            print('WA!')
            print(a)
            print('\n\n\n\n\n')
            print(b)
            print('\n\n\n\n\n')
            print(run_solve(a, b))
            exit(0)
        print(str(tests_num) + ' passed. : ' + str(a) + ' * ' + str(b))


if __name__ == '__main__':
    main()

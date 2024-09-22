using System;
using System.Text;

namespace RSA2._0
{
    internal class Program
    {

        /// 
        /// 
        /// Helping Functions
        public static bool IsPrime(long n)
        {
            if (n % 2 == 0)
            {
                return false;
            }
            for (int i = 3; i <= Math.Sqrt(n); i += 2)
            {
                if (n % i == 0)
                {
                    return false;
                }
            }
            return true;
        }
        /// 
        /// 
        /// 


        //Step 1 generate all prime numbers to 1000000
        static bool[] PrimeNumbersArrayGenerator() //Sieve of Eratosthenes
        {
            int N = 100000000;
            bool[] prime = new bool[N];
            for (int i = 0; i < N; i++)
            {
                prime[i] = true;
            }
            prime[0] = false;
            prime[1] = false;
            for (int i = 2; i < Math.Ceiling(Math.Sqrt(N)); i++)
            {
                if (prime[i])
                {
                    int j = i * i;
                    while (j < N)
                    {
                        prime[j] = false;
                        j += i;
                    }
                }
            }
            return prime;
        }


        // Step 2 Choose two big random prime numbers
        static void ChoosePandQ(ref long p, ref long q, bool[] primeNumbers)
        {
            List<long> PrimeNumbers = new();
            Random random = new();
            for (long i = 1000; i <= 10000;  i++)
            {
                if (primeNumbers[i])
                {
                    PrimeNumbers.Add(i);
                }
            }
            while (true)
            {
                p = PrimeNumbers[random.Next(PrimeNumbers.Count)];
                q = PrimeNumbers[random.Next(PrimeNumbers.Count)];
                if (p != q)
                {
                    break;
                }
            }
        }

        //Greatest common divider function
        static long GCD(long n1, long n2)
        {
            if (n2 == 0)
            {
                return n1;
            }
            else
            {
                return GCD(n2, n1 % n2); 
            }
        }

        //Check mutual Simplicity using gcd
        static bool CheckMutualSimplicity(long n1, long n2)
        {
            if (GCD(n1, n2) == 1)
            {
                return true;
            }
            else { return false; }
        }

        //Generate e
        static long EGenerator(long limit)
        {
            Random randomizer = new();
            long e = randomizer.NextInt64(1, limit);
            bool run = true;
            while (run)
            {
                if (CheckMutualSimplicity(e, limit))
                {
                    run = false; break;
                }
                else
                {
                    e = randomizer.NextInt64(1, limit);
                }
            }
            return e;
        }

        public static (long, long, long) EuclidEX(long a, long b)
        {
            if (b == 0)
            {
                return (a, 1, 0);
            }

            var (d, x, y) = EuclidEX(b, a % b);
            return (d, y, x - (a / b) * y);
        }

        static long GetD(long e, long Euler)
        {
            var results = EuclidEX(e, Euler);
            long a_1 = results.Item2;
            long d = a_1 % Euler;
            while (d < 0)
                d += Euler;
            while (d > Euler)
                d -= Euler;
            return d;
        }
        static long ModPow(long baseValue, long exponent, long modulus)
        {
            if (modulus == 1) return 0;
            long result = 1;
            baseValue %= modulus;
            while (exponent > 0)
            {
                if ((exponent & 1) == 1)
                    result = (result * baseValue) % modulus;
                exponent >>= 1;
                baseValue = (baseValue * baseValue) % modulus;
            }
            return result;
        }

        static string encryption(string text, long e, long N)
        {
            StringBuilder result = new StringBuilder();
            foreach (char c in text)
            {
                result.Append($"{ModPow((long)c, e, N)} ");
            }
            return result.ToString();
        }

        static string decryption(string entext, long d, long N)
        {
            string[] Entext = entext.Split(' ');
            string result = "";
            long value;
            for(int i = 0; i < Entext.Length-1; i++)
            {
                value = long.Parse(Entext[i]);
                result += ((char)ModPow(value, d, N));

            }
            return result.ToString();
        }

        static void FullEncryption(string text)
        {
            bool[] prime = PrimeNumbersArrayGenerator(); // Checked till 1000
            long p = 0, q = 0;
            ChoosePandQ(ref p, ref q, prime);
            long N = p * q;
            long Euler = (p - 1) * (q - 1);
            long e = EGenerator(Euler);
            long d = GetD(e, Euler);
            string result = encryption(text, e, N);
            using (StreamWriter writer = new StreamWriter("privateKey.txt"))
            {
                writer.WriteLine($"{d} {N}");
            }
            File.WriteAllText("Ciphertext.txt", result);
        }
        static void FullDecryption()
        {
            string text = File.ReadAllText("Ciphertext.txt");
            long d, N;
            string[] values = File.ReadAllText("privateKey.txt").Split(' ');
            d = long.Parse(values[0]);
            N = long.Parse(values[1]);
            string result = decryption(text, d, N);
            File.WriteAllText("Decrypted.txt", result);
        }
        static void Main(string[] args)
        {
            Console.OutputEncoding = Encoding.UTF8;
            Random random = new();
            bool run = true;
            try
            {
                checked
                {


                    string menu = "\n1. Encrypt text\nThe text will be read from a file named (Plaintext.txt)\nThe encrypted text and keys (Ciphertext.txt; privateKey.txt) will be saved in the folder where the program is located\n\n\n2. Decrypt text\nThe text will be read from a file named (Ciphertext.txt)\nThe private key will be read from the file (privateKey.txt)\nThe result will be saved to the file (Decrypted.txt)\n\n3.Exit\n";
                    while (run)
                    {
                        Console.WriteLine(menu);
                        Console.WriteLine("Choose an operation mode");
                        int choice = int.Parse(Console.ReadLine());
                        switch (choice)
                        {
                            case 1:
                                string text = File.ReadAllText("Plaintext.txt");
                                FullEncryption(text);
                                Console.WriteLine("Text encrypted!");
                                break;
                            case 2:
                                FullDecryption();
                                Console.WriteLine("Text decrypted successfully");
                                break;
                            case 3:
                                run = false;
                                break;
                            default:
                                Console.WriteLine("Input error");
                                break;
                        }
                    }

                }
            }
            catch (OverflowException ex)
            {
                Console.WriteLine("Overflow detected: " + ex.Message);
            }
        }
    }
}

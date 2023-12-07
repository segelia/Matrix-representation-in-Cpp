#include "Matrix.h"

int main()
{
    try
    {
        Matrix mat1(2, 2);
        Matrix mat2(2, 2);
        Matrix mat3(2, 3);
        
        //mat1(0,0) = 12;	// Write operation.
        //int x = mat1(0,0); // Read operation.

        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                mat1(i, j) = i + j;
                mat2(i, j) = i * j;
            }
        }

        std::cout << "Matrix 1:\n"
                  << mat1 << std::endl;
        std::cout << "Matrix 2:\n"
                  << mat2 << std::endl;


        Matrix additionResult = mat1 + mat2;
        std::cout << "Matrix 1 + Matrix 2:\n"
                  << additionResult << std::endl;

        Matrix subtractionResult = mat1 - mat2;
        std::cout << "Matrix 1 - Matrix 2:\n"
                  << subtractionResult << std::endl;

        // Matrix multiplicationResult = mat3 * mat1; // Expecting an exception here due to incompatible dimensions
        // std::cout << "Matrix 1 * Matrix 3:\n"
        //           << multiplicationResult << std::endl;

        mat1 += mat2;
        std::cout << "Matrix 1 after += Matrix 2:\n"
                  << mat1 << std::endl;

        mat1 -= mat2;
        std::cout << "Matrix 1 after -= Matrix 2:\n"
                  << mat1 << std::endl;

        if (mat1 == mat2)
        {
            std::cout << "Matrix 1 is equal to Matrix 2.\n";
        }
        else
        {
            std::cout << "Matrix 1 is not equal to Matrix 2.\n";
        }

        if (mat1 != mat2)
        {
            std::cout << "Matrix 1 is not equal to Matrix 2.\n";
        }
        else
        {
            std::cout << "Matrix 1 is equal to Matrix 2.\n";
        }
    }
    catch (const MatrixError &e)
    {
        std::cerr << "Matrix Error: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

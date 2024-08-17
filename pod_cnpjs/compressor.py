import gzip
import shutil
import os

def compress_file(input_file, output_file):
    with open(input_file, 'rb') as f_in:
        with gzip.open(output_file, 'wb') as f_out:
            shutil.copyfileobj(f_in, f_out)

def calculate_compression_ratio(original_file, compressed_file):
    original_size = os.path.getsize(original_file)
    compressed_size = os.path.getsize(compressed_file)
    return compressed_size / original_size

def main():
    input_file = 'Empresas0.EMPRECSV'
    output_file = 'Empresas0_compressed.gz'
    
    # Compress the file
    compress_file(input_file, output_file)
   
    # Calculate and print the compression ratio
    ratio = calculate_compression_ratio(input_file, output_file)
    print(f'Compression ratio: {ratio:.2f}')

    # Check if the compression ratio meets the requirement
    if ratio <= 0.30:
        print('Compression ratio meets the requirement.')
    else:
        print('Compression ratio does not meet the requirement.')

if __name__ == '__main__':
    main()

import sys, getopt

def main(argv):
    outputfile = 'stdin'
    contents = ''
    try:
        opts, args = getopt.getopt(argv,"c:f:",["ifile=","ofile="])
    except getopt.GetoptError:
        print("A getopt error in the mail")
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-f':
            outputfile = arg
        elif opt == '-c':
            contents = arg

    # print("The output file is ",outputfile)
    # print("the content is ",contents)

    #open the file and attempt to write

    foutfle = open(outputfile,"w")
    #over write or create and write
    foutfle.write(contents + "\n")
    #close file
    foutfle.close()


if __name__ == "__main__":
    main(sys.argv[1:])
            

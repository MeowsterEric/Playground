# get/convert data from internet
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import numpy as np
import urllib


def bytespdate2num(fmt, encoding='utf-8'):
    strconverter = mdates.strpdate2num(fmt)

    def bytesconverter(b):
        s = b.decode(encoding)
        return strconverter(s)
    return bytesconverter


# getting data via Yahoo Finance API
def graph_data(stock):

    URL = "http://chartapi.finance.yahoo.com/instrument/1.0/%s/chartdata;type=quote;range=3y/csv" % (stock)
    source_code = urllib.request.urlopen(URL).read().decode()

    stock_data = []
    split_source = source_code.split("\n")

    for line in split_source:
        # hard code to get rid of header
        if ":" not in line:
            stock_data.append(line)

    # bytespdate2num function format
    # %Y = Full year 2017
    # %y = partial year 17
    # %m = number month
    # %d = number day
    # %H = hours
    # %M = minutes
    # %S = seconds
    date, close_price, high_price, low_price, open_price, volume = np.loadtxt(
        stock_data, delimiter=",", unpack=True, converters={0: bytespdate2num('%Y%m%d')})

    plt.plot_date(date, close_price, '-', label="Price")
    plt.xlabel("Date")
    plt.ylabel("Price ($)")
    plt.title('Yahoo Finance')
    plt.legend()
    plt.show()  # show the drawing

graph_data("TSLA")

/******************************************************************************

The MIT License (MIT)

Copyright (c) 2018 cqwrteur

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

******************************************************************************/


#include<fstream>
#include<iostream>
#include<boost/property_tree/json_parser.hpp>
#include<boost/multiprecision/cpp_int.hpp>
#include<boost/multiprecision/cpp_dec_float.hpp>
#include<vector>

int main(int argc,char **argv)
try
{
	if(argc!=2)
	{
		std::cerr<<"Usage: "<<*argv<<" <filename>\n";
		return 1;
	}
	std::ifstream fin(argv[1],std::ifstream::binary);
	boost::property_tree::ptree pt;
	boost::property_tree::read_json(fin, pt);
	
	const auto compensation(*----pt.end());	//NO THIS IS NOT LABOR. HE IS FAKING DATA.
	const auto output(pt.back());
	std::vector<std::pair<boost::multiprecision::cpp_rational,boost::multiprecision::cpp_rational>> vec;
	for(auto i(++compensation.second.begin()),j(++output.second.begin());i!=compensation.second.end();++i,++j)
		vec.emplace_back(std::piecewise_construct,std::forward_as_tuple(i->second.get<std::string>("")),
		std::forward_as_tuple(j->second.get<std::string>("")));
	boost::multiprecision::cpp_rational x(0),y(0),xx(0),xy(0);
	for(const auto &ele : vec)
	{
		x+=ele.first;
		y+=ele.second;
		xx+=ele.first*ele.first;
		xy+=ele.first*ele.second;
	}
	auto n(vec.size());
	auto b((n*xy-x*y)/(n*xx-x*x));
	auto a(y/n-b*x/n);
	auto xbar(x/n),ybar(y/n);
	decltype(x) xd(0),yd(0);

	decltype(x) ssr(0);
	
	for(const auto &ele : vec)
	{
		ssr+=(ele.first-xbar)*(ele.second-ybar);
		xd+=(ele.first-xbar)*(ele.first-xbar);
		yd+=(ele.second-ybar)*(ele.second-ybar);
	}
	std::cout<<"y="<<boost::multiprecision::cpp_dec_float_100(a)<<"+"<<boost::multiprecision::cpp_dec_float_100(b)<<"x\n";
	boost::multiprecision::cpp_dec_float_100 r(ssr*ssr/(xd*yd));
	std::cout<<"r="<<r<<'\n';
}
catch(const std::exception& e)
{
	std::cerr<<e.what()<<'\n';
	return 1;
}
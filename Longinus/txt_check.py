# -*- coding:utf-8 -*-
#用于生成日中字符串对或生成Longinus.dat
#made by Darkness-TX
#2018.01.25
import struct
import os
import sys
import io

def long2int(byte):
    return struct.unpack('L',byte)[0]

def int2long(num):
    return struct.pack('L',num)

def check(name, num, dic, dst=''):
	old_src = open('txt_old/' + name,'r',encoding='utf16')
	new_src = open('txt_new/' + name,'r',encoding='utf16')
	old_txt = old_src.readlines()
	new_txt = new_src.readlines()
	if len(old_txt) != len(new_txt):
		print('文件行数不统一！')
		os.system('pause')
		sys.exit()
	for i in range(0,len(old_txt)):
		if old_txt[i] != new_txt[i]:
			#dst.write('OldlpString' + str(num) + '=' + old_txt[i])
			#dst.write('NewlpString' + str(num) + '=' + new_txt[i])
			num += 1
			if old_txt[i] not in dic:
				dic[old_txt[i].strip()] = new_txt[i].strip()

	old_src.close()
	new_src.close()
	del(old_txt)
	del(new_txt)
	return num

def dict_out(dic):
	num = 1
	dst = open('dict_out.txt','w',encoding='utf16')
	for i in dic:
		#dst.write('OldlpString' + str(num) + '=' + i)
		#dst.write('NewlpString' + str(num) + '=' + dic[i])
		dst.write(i + '\n')
		dst.write(dic[i] + '\n')
		num += 1
	dst.close()

def hash_check(dic,num):
	hash_list = []
	for i in dic:
		hash_list.append(BKDRHash(i))
	if len(set(hash_list)) == len(dic):
		print('hash运算后无重复!')
		return True
	else:
		print('hash运算后有重复!\n')
		print('hash运算后去重:' + str(len(set(hash_list))),'字典长:' + str(len(dic)),'num:' + str(num))
		os.system('pause')
		return False

#来自《The C Programming Language》，所以叫BKDRHash，
#经测试重复率几乎0，应付汉化应该没问题
def BKDRHash(string):
	seed = 131
	hash = 0
	for ch in string.encode('utf16')[2:]:
		hash = hash * seed + ch
	return hash & 0x7FFFFFFF

def hash_dict_build(dic,out = False):
	hash_dic = {}
	if out:
		dst = open('hash_dict.txt','w',encoding='utf16')
	for i in dic:
		hash_dic['%08X' % BKDRHash(i)] = dic[i]
		if out:
			dst.write('%08X|%s\n' % (BKDRHash(i),dic[i]))
	if out:
		dst.close()
	return hash_dic

def Longinus_dat_build(dic):
	dst = open('Longinus.dat','wb')
	dst.write('Longinus'.encode('utf8'))
	dst.write(b'\x01\x03\x00')#版本号1.3.0
	key_list = list(dic.keys())
	first_list = []
	for i in key_list:
		first_list.append(i[:2])
	count_dict = {}
	for i in set(first_list):
		count = 0
		for k in first_list:
			if k == i:
				count += 1
		count_dict[i] = count
	del(first_list)
	#hash开头一字节先组个索引，这里一般也就100多个，基于索引再去匹配整个hash，这里就看文本量多少了，
	#但是一般这样拆分后都很平均地分布在这100多个中，去重后几万行的文本最坏情况基本合计只用循环搜索几百就能找到，
	#因为Longinus全用纯C写的所以都要自己实现，嫌麻烦的自己找更好的算法或者直接保存原始和汉化后的字符串用C艹的STL map算好等方法
	dst.write(bytes((len(count_dict) - 1,)))#第一段索引的数量
	dst.seek(0x10,os.SEEK_SET)#预留4字节
	count_dict = sorted(count_dict.items(), key=lambda d:d[1], reverse = True)#从多到少的排序
	for i in count_dict:
		dst.write(struct.pack('B',int(i[0],16)))
		dst.write(int2long(i[1])[:-1])#三字节还不够你要搞啥？
		for k in key_list:
			if k[:2] == i[0]:
				str_byte = dic[k].encode('utf16')[2:]
				dst.write(struct.pack('H',len(str_byte)))#不可能有一句话大于65535的吧！
				dst.write(int2long(int(k,16)))
				dst.write(str_byte)
				dic.pop(k)
	print('创建完成!')
	dst.close()


def main():
	dic = {}
	num = 1
	#dst = open('txt_out.txt','w',encoding='utf16')
	for f in os.listdir('txt_old'):
		if not f.endswith('.txt'):
			continue
		num = check(f, num, dic)#, dst)
	#dict_out(dic)
	if hash_check(dic,num):
		dic = hash_dict_build(dic,True)
	Longinus_dat_build(dic)
main()
# -*- coding:utf-8 -*-
#用于V4版的Longinus.dat
#made by Darkness-TX
#2018.04.10
import struct
import os
import sys
import io

def long2int(byte):
	return struct.unpack('L',byte)[0]

def int2long(num):
	return struct.pack('L',num)

def hash_check(dic,num):
	hash_list = []
	for name in dic:
		hash_list.append(BKDRHash(name.upper()))
	if len(set(hash_list)) == len(dic):
		print('hash运算后无重复!')
		return True
	else:
		print('hash运算后有重复!\n')
		print('hash运算后去重:' + str(len(set(hash_list))),'字典长:' + str(len(dic)),'num:' + str(num))
		os.system('pause')
		return False

def hash_dict_build(dic,out = False):
	hash_dic = {}
	if out:
		dst = open('hash_dict.txt','w',encoding='utf16')
	for name in dic:
		hash_dic[name] = '%08X|%08X' % (BKDRHash(name.upper()), dic[name])
		if out:
			dst.write('%s|%s\n' % (name.upper(), hash_dic[name]))
	if out:
		dst.close()
	return hash_dic

#来自《The C Programming Language》，所以叫BKDRHash，
#经测试重复率几乎0，应付汉化应该没问题
def BKDRHash(string):
	seed = 131
	hash = 0
	for ch in string.encode('utf16')[2:]:
		hash = hash * seed + ch
	return hash & 0x7FFFFFFF

def Longinus_dat_build(hash_dic, num):
	dst = open('Longinus.dat','wb')
	dst.write('Longinus'.encode('utf8'))
	dst.write(b'\x01\x04\x00')#版本号1.4.0
	dst.write(b'\x01')#1.4.0版本后增加的，0是文本包，1是文件包，这里暂时只写文件包的封法
	dst.write(int2long(num))#文件数，先占用4字节
	#这里一般封包最多也就200个文件这样吧，就不对Hash进行分组了直接循环吧。。。。。
	i = 0
	#开头是索引：Hash值,size,offset。共0x0C字节
	dst.seek(0x10 + num * 0x0C, os.SEEK_SET)
	for name in hash_dic:
		offset = dst.tell()
		data = open('build/' + name, 'rb').read()
		dst.write(data)
		dst.seek(0x10 + i * 0x0C, os.SEEK_SET)
		rows = hash_dic[name].split('|')
		dst.write(int2long(int(rows[0],16)))
		dst.write(int2long(int(rows[1],16)))
		dst.write(int2long(offset))
		dst.seek(0, os.SEEK_END)
		i += 1
	print('创建完成!')
	dst.close()

def main():
	dic = {}
	num = 0
	for f in os.listdir('build'):
		size = os.path.getsize('build/' + f)
		dic[f] = size
		num += 1
	if hash_check(dic,num):
		dic = hash_dict_build(dic,True)
		Longinus_dat_build(dic, num)
main()
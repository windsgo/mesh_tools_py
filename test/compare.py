from mslice import *
  
if __name__ == "__main__":
    argc = len(sys.argv)
    if (argc != 7):
        print("Usage python3 (dir/)test.py [file1] [file2] [a] [b] [c] [d]")
        exit(1)
      
    mesh_file1 = sys.argv[1]
    mesh_file2 = sys.argv[2]
    a_ = float(sys.argv[3])
    b_ = float(sys.argv[4])
    c_ = float(sys.argv[5])
    d_ = float(sys.argv[6])
    
    
    
    try:
        mesh1 = mesh_slicer(mesh_file1) # use False here to disable print!
        print("mesh_slicer ctor success")
    except:
        print("mesh_slicer ctor exception", mesh_file1)
        exit(1)


    try:
        mesh2 = mesh_slicer(mesh_file2) # use False here to disable print!
        print("mesh_slicer ctor success")
    except:
        print("mesh_slicer ctor exception", mesh_file2)
        exit(1)

    # test save
    '''
    mesh1.save_intersection_polylines(a_, b_, c_, d_, 
                                  '{}_intersection_{}x_{}y_{}z_{}.txt'.format(mesh_file1, a_, b_, c_ , d_),
                                  ',')
    '''

    # test get face indices
    # faces = mesh1.get_original_faces()
    # print("faces: ", len(faces))

    if False:
        with open('faces.txt', 'w') as f:
            for line in faces:
                for axis in line:
                    print(axis, ',', file=f, sep='', end='')
                print('', file=f)
    
    # example to directly get data
    try:
        line1 : np.ndarray = mesh1.get_intersection_polylines(a_, b_, c_, d_)
    except:
        print("get_intersection_polylines line1 exception")
        exit(1)

    try:
        line2 : np.ndarray = mesh2.get_intersection_polylines(a_, b_, c_, d_)
    except:
        print("get_intersection_polylines line2 exception")
        exit(1)
        
    
        
    # test    
    print("等高线段数1:", len(line1)) # 等高线段数
    print("等高线段数2:", len(line2)) # 等高线段数
    
    if len(line1) == 0 or len(line2) == 0:
        exit(1)

    # first_group : np.ndarray = line1[0] # 获取第一段等高线
    # print("等高线数据类型:", type(first_group)) # 每一段是一个numpy二维数组
    # print("一段等高线的数组维度:", first_group.shape) # 维度

    # first_point_in_first_group : np.ndarray = first_group[0] # 第一段等高线的第一个点
    # print("点数据类型:", type(first_point_in_first_group)) 
    # print("点的数组维度:", first_point_in_first_group.shape) 

    # print(first_group.size/3)
    
    ## plot
    fig = plt.figure()
    
    ## 2d
    ax_2d = plt.axes()
    for group in line1:
        ax_2d.plot(group[:,0], group[:,1], color='green', marker='.', linestyle='-', linewidth=1, markersize=2, label=mesh_file1)
    
    for group in line2:
        ax_2d.plot(group[:,0], group[:,1], color='red', marker='.', linestyle='-', linewidth=1, markersize=2, label=mesh_file2)
        
    plt.axis("equal")
    plt.xlabel("x")
    plt.ylabel("y")
    # 先保存一张二维图
    plt.title('compare at {}x + {}y + {}z = {}'.format(a_, b_, c_ , d_))
    
    plt.legend()
    plt.grid(True)
    
    plt.xlabel("x")
    plt.ylabel("y")

    # plt.savefig('{}_intersection_{}x_{}y_{}z_{}.png'.format(mesh_file1, a_, b_, c_ , d_), dpi=900)
    plt.show()
    
    # exit(0)

    # ## 3d
    # ax = plt.axes(projection='3d')
    # # fig = plt.figure()
    # # ax = fig.add_subplot(projection='3d')
    # for group in x:
    #     # if len(fig.axes) < 1:
    #     #     fig.add_subplot(projection='3d')
    #     # ax = fig.gca()
    #     # ax1 = fig.add_subplot(ax)
    #     # print(type(ax) == type(ax1))
    #     ax.plot(group[:,0], group[:,1], group[:,2], linewidth=2.5, linestyle='solid')
    
    
    # ori_points : np.ndarray = a.get_original_vertices()
    # print(ori_points.shape)
    
    # ## 对原始数据筛选z附近
    # '''
    # delete_indexes = []
    # delete_range : int = 50 # 设置绘图点在z附近的范围
    # for i in range(ori_points.shape[0]):
    #     if ori_points[i,2] > z + delete_range or ori_points[i,2] < z - delete_range:
    #         delete_indexes.append(i)

    # # delete_indexes = [] 
    # m = np.delete(ori_points, delete_indexes, axis=0) # 一次性进行筛选，尚不明确这样删除的内存开销
    # print(m.shape)
    # '''
    # m = ori_points
    # down_s_ratio = 10 # 绘图间隔点，用于节省绘图开销
    # # ax.scatter(m[::down_s_ratio,0], m[::down_s_ratio,1], m[::down_s_ratio,2], s=0.1, marker='.', alpha=0.4)
    # ax.plot(m[::down_s_ratio,0], m[::down_s_ratio,1], m[::down_s_ratio,2], marker='.', alpha=1.0, linestyle="", markersize=1)
    
    # plt.xlabel("x")
    # plt.ylabel("y")
    # try:
    #     plt.axis('tight')
    #     plt.axis('equal')
    # except:
    #     pass
    # plt.title('{}_intersection_3d_{}x_{}y_{}z_{}'.format(mesh_file, a_, b_, c_ , d_))
    # plt.savefig('{}_intersection_3d_{}x_{}y_{}z_{}.png'.format(mesh_file, a_, b_, c_ , d_), dpi=900)
    
    # plt.show()
    
    
  